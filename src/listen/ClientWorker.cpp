#include "ClientWorker.h"

#include <vector>

#include "constant/ConfigConstant.h"
#include "constant/PropertyKeyConst.h"
#include "listen/Listener.h"
#include "src/config/ConfigProxy.h"
#include "src/crypto/md5/md5.h"
#include "src/debug/DebugAssertion.h"
#include "src/http/HttpStatus.h"
#include "src/log/Logger.h"
#include "src/utils/GroupKey.h"
#include "src/utils/ParamUtils.h"
#include "src/utils/TimeUtils.h"
#include "src/utils/url.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))

using namespace std;

namespace nacos {
ClientWorker::ClientWorker(ObjectConfigData *objectConfigData) {
    threadId = 0;
    stopThread = true;
    pthread_mutex_init(&watchListMutex, NULL);
    pthread_mutex_init(&stopThreadMutex, NULL);
    _objectConfigData = objectConfigData;

    _longPullingTimeoutStr = _objectConfigData->_appConfigManager->get(
        PropertyKeyConst::CONFIG_LONGPULLLING_TIMEOUT);
    _longPullingTimeout = atoi(_longPullingTimeoutStr.c_str());
}

ClientWorker::~ClientWorker() {
    log_debug("[ClientWorker]:~ClientWorker()\n");
    stopListening();
    cleanUp();
}

NacosString ClientWorker::getServerConfig(const NacosString &tenant,
                                          const NacosString &dataId,
                                          const NacosString &group,
                                          long timeoutMs)
    NACOS_THROW(NacosException) {
    HttpResult res = getServerConfigHelper(tenant, dataId, group, timeoutMs);
    saveConfigToLocalFile(tenant, dataId, group, res);
    return res.content;
}

HttpResult ClientWorker::getServerConfigHelper(const NacosString &tenant,
                                               const NacosString &dataId,
                                               const NacosString &group,
                                               long timeoutMs)
    NACOS_THROW(NacosException) {
    std::list<NacosString> headers;
    std::list<NacosString> paramValues;

    ParamUtils::addKV(paramValues, "dataId", dataId);

    if (!isNull(group)) {
        ParamUtils::addKV(paramValues, "group", group);
    } else {
        ParamUtils::addKV(paramValues, "group", ConfigConstant::DEFAULT_GROUP);
    }

    if (!isNull(tenant)) {
        ParamUtils::addKV(paramValues, "tenant", tenant);
    }

    // Get the request url
    NacosString path = _objectConfigData->_appConfigManager->getContextPath() +
                       ConfigConstant::CONFIG_CONTROLLER_PATH;
    NacosString serverAddr =
        _objectConfigData->_serverListManager->getCurrentServerAddr();
    NacosString url = serverAddr + "/" + path;
    log_debug("[ClientWorker]-getServerConfigHelper:httpGet Assembled URL:%s\n",
              url.c_str());

    HttpResult res;
    ConfigProxy *_configProxy = _objectConfigData->_configProxy;
    try {
        res = _configProxy->reqAPI(IHttpCli::GET, url, headers, paramValues,
                                   _objectConfigData->encoding, timeoutMs);
    } catch (NetworkException &e) {
        throw NacosException(NacosException::SERVER_ERROR, e.what());
    }

    return res;
}

void *ClientWorker::listenerThread(void *parm) {
    log_debug("[ClientWorker]-listenerThread:Entered watch thread...\n");
    ClientWorker *thelistener = (ClientWorker *)parm;

    while (!thelistener->stopThread) {
        int64_t start_time = TimeUtils::getCurrentTimeInMs();
        log_debug("[ClientWorker]-listenerThread:Start watching at %u...\n",
                  start_time);
        thelistener->performWatch();

        log_debug(
            "[ClientWorker]-listenerThread:Watch function exit at %u...\n",
            TimeUtils::getCurrentTimeInMs());
    }

    return 0;
}

vector<NacosString> ClientWorker::parseListenedKeys(
    const NacosString &ReturnedKeys) {
    NacosString changedKeyList = urldecode(ReturnedKeys);

    vector<NacosString> explodedList;
    ParamUtils::Explode(explodedList, changedKeyList,
                        ConfigConstant::LINE_SEPARATOR);

    // If the server returns a string with a trailing \x01, actually there is no
    // data after that but ParamUtils::Explode will return an extra item with
    // empty string, we need to remove that from the list so it won't disrupt
    // subsequent operations
    log_debug("[ClientWorker]-parseListenedKeys:extra data:%s\n",
              explodedList[explodedList.size() - 1].c_str());
    if (explodedList.size() >= 1 &&
        ParamUtils::isBlank(explodedList[explodedList.size() - 1])) {
        explodedList.pop_back();
    }
    return explodedList;
}

void ClientWorker::startListening() {
    // Already started, skip this
    if (!stopThread) {
        log_debug(
            "[ClientWorker]-startListening:The thread is already started or "
            "the starting is in progress...\n");
        return;
    }

    pthread_mutex_lock(&stopThreadMutex);
    if (!stopThread) {
        pthread_mutex_unlock(&stopThreadMutex);
        log_debug(
            "[ClientWorker]-startListening:The thread is already started or "
            "the starting is in progress...\n");
        return;
    }

    stopThread = false;
    pthread_mutex_unlock(&stopThreadMutex);

    log_debug("[ClientWorker]-startListening:Starting the thread...\n");
    pthread_create(&threadId, NULL, listenerThread, (void *)this);
    log_debug("[ClientWorker]-startListening:Started thread with id:%d...\n",
              threadId);
}

void ClientWorker::stopListening() {
    log_debug("[ClientWorker]-stopListening: entered\n");
    if (stopThread)  // Stop in progress
    {
        log_debug(
            "[ClientWorker]-stopListening:The thread is already stopped or the "
            "stop is in progress...\n");
        return;
    }

    pthread_mutex_lock(&stopThreadMutex);
    if (stopThread)  // Stop in progress
    {
        pthread_mutex_unlock(&stopThreadMutex);
        log_debug(
            "[ClientWorker]-stopListening:The thread is already stopped or the "
            "stop is in progress...\n");
        return;
    }

    stopThread = true;
    pthread_mutex_unlock(&stopThreadMutex);

    pthread_join(threadId, NULL);
    log_info(
        "[ClientWorker]-stopListening:The thread is stopped successfully...\n");
}

void ClientWorker::addListener(const NacosString &dataId,
                               const NacosString &group,
                               const NacosString &tenant,
                               const NacosString &initialContent,
                               Listener *listener) {
    NacosString key = GroupKey::getKeyTenant(dataId, group, tenant);
    log_debug("[ClientWorker]-addListener:Adding listener with key: %s\n",
              key.c_str());
    pthread_mutex_lock(&watchListMutex);

    // Check whether the listener being added to the list already exists
    if (listeningKeys.find(key) != listeningKeys.end()) {
        ListeningData *curListeningData = listeningKeys[key];
        if (!curListeningData->addListener(listener)) {
            log_warn(
                "[ClientWorker]-addListener:Key %s is already in the watch "
                "list, leaving...\n",
                key.c_str());
        }
        listener->incRef();
        pthread_mutex_unlock(&watchListMutex);
        return;
    }

    // if the listener does not exist, just create it

    ListeningData *listeningData =
        new ListeningData(tenant, dataId, group, initialContent);

    // If no, copy one
    listeningKeys[key] = listeningData;
    listeningData->addListener(listener);
    listener->incRef();
    pthread_mutex_unlock(&watchListMutex);
    log_debug("[ClientWorker]-addListener:Key %s is added successfully!\n",
              key.c_str());
}

/**
 * Removes a listener from the listened list actively, but slow
 * @param listener the listener to be removed
 * @author Liu, Hanyu
 */
void ClientWorker::removeListenerActively(const NacosString &dataId,
                                          const NacosString &group,
                                          const NacosString &tenant,
                                          Listener *listener) {
    NacosString key = GroupKey::getKeyTenant(dataId, group, tenant);
    pthread_mutex_lock(&watchListMutex);
    map<NacosString, ListeningData *>::iterator it = listeningKeys.find(key);
    // Check whether the cachedata being removed exists
    if (it == listeningKeys.end()) {
        log_warn(
            "[ClientWorker]-removeListenerActively:Removing a non-existing "
            "listener %s, leaving...\n",
            key.c_str());
        pthread_mutex_unlock(&watchListMutex);
        return;
    }

    // If so, remove it and free the resources
    ListeningData *curListeningData = it->second;
    bool succRemoved = curListeningData->removeListener(listener);
    if (!succRemoved) {
        log_warn(
            "[ClientWorker]-removeListenerActively:Removing a non-existing "
            "listener %s...\n",
            key.c_str());
    } else {
        // remove the listener, it is created by the client but freed by
        // nacos-sdk-cpp
        log_debug(
            "[ClientWorker]-removeListenerActively:Removing a listener %s...\n",
            key.c_str());
        int refcount = listener->decRef();
        if (refcount == 0) {
            log_debug(
                "[ClientWorker]-removeListenerActively:"
                "Refcount of the listener(Name = %s) is 0 so delete it.\n",
                listener->getListenerName().c_str());
            delete listener;
            listener = NULL;
        }
    }

    // no listener on the list, remove the entry
    if (curListeningData->isEmpty()) {
        listeningKeys.erase(key);
        // free the space for this slot
        delete curListeningData;
    }
    pthread_mutex_unlock(&watchListMutex);
}

/**
 * Removes a listener from the listened list(very fast compared to
 * removeListenerActively) This function will return fast, since it just mark
 * the remove flag of the listener and wait for the watcher to actually remove
 * it
 * @param listener the listener to be removed
 * @author Liu, Hanyu
 */
void ClientWorker::removeListener(const NacosString &dataId,
                                  const NacosString &group,
                                  const NacosString &tenant,
                                  Listener *listener) {
    // set the remove flag and return quickly
    // the background process will remove the listener
    // add this listener to the remove list since it is marked to be removed
    OperateItem operateItem(dataId, tenant, group, listener);
    addDeleteItem(operateItem);
}

NacosString ClientWorker::checkListenedKeys()
    NACOS_THROW(NetworkException, NacosException) {
    NacosString postData;
    pthread_mutex_lock(&watchListMutex);
    for (map<NacosString, ListeningData *>::iterator it = listeningKeys.begin();
         it != listeningKeys.end(); it++) {
        ListeningData *curListenedKey = it->second;

        postData += curListenedKey->getDataId();
        postData += ConfigConstant::WORD_SEPARATOR;
        postData += curListenedKey->getGroup();
        postData += ConfigConstant::WORD_SEPARATOR;

        if (!isNull(curListenedKey->getTenant())) {
            postData += curListenedKey->getMD5();
            postData += ConfigConstant::WORD_SEPARATOR;
            postData += curListenedKey->getTenant();
            postData += ConfigConstant::LINE_SEPARATOR;
        } else {
            postData += curListenedKey->getMD5();
            postData += ConfigConstant::LINE_SEPARATOR;
        }
    }
    pthread_mutex_unlock(&watchListMutex);

    list<NacosString> headers;
    list<NacosString> paramValues;

    headers.push_back("Long-Pulling-Timeout");
    headers.push_back(_longPullingTimeoutStr);

    paramValues.push_back(ConfigConstant::PROBE_MODIFY_REQUEST);
    paramValues.push_back(postData);
    log_debug("[ClientWorker]-checkListenedKeys:Assembled postData:%s\n",
              postData.c_str());

    // Get the request url
    // TODO:move /listener to constant
    NacosString path = _objectConfigData->_appConfigManager->getContextPath() +
                       ConfigConstant::CONFIG_CONTROLLER_PATH + "/listener";
    HttpResult res;

    NacosString serverAddr =
        _objectConfigData->_serverListManager->getCurrentServerAddr();
    NacosString url = serverAddr + "/" + path;
    log_debug("[ClientWorker]-checkListenedKeys:httpPost Assembled URL:%s\n",
              url.c_str());

    ConfigProxy *_configProxy = _objectConfigData->_configProxy;
    res =
        _configProxy->reqAPI(IHttpCli::POST, url, headers, paramValues,
                             _objectConfigData->encoding, _longPullingTimeout);
    log_debug(
        "[ClientWorker]-checkListenedKeys:Received the message below from "
        "server:\n%s\n",
        res.content.c_str());
    log_debug(
        "[ClientWorker]-checkListenedKeys:return status:httpcode=%d "
        "curlcode=%d\n",
        res.code, res.curlcode);

    if (res.code != HttpStatus::HTTP_OK) {
        // bugfix #52 please check github
        throw NacosException(res.code, "Error while watching keys");
    }

    return res.content;
}

void ClientWorker::performWatch() {
    MD5 md5;
    NacosString changedData;
    try {
        changedData = checkListenedKeys();
    } catch (NetworkException &e) {
        log_warn(
            "[ClientWorker]-performWatch:sleep and retry, reason: "
            "NetworkException with: %s\n",
            e.what());
        // wait for at lease 3 secs to avoid too many wake-ups when the network
        // is down
        sleep(MAX(_longPullingTimeout / 10000, 3));
        return;
    } catch (NacosException &e) {
        log_warn(
            "[ClientWorker]-performWatch:sleep and retry, reason: "
            "NacosException with: %s\n",
            e.what());
        // same reason as above
        sleep(MAX(_longPullingTimeout / 10000, 3));
        return;
    }

    vector<NacosString> changedList =
        ClientWorker::parseListenedKeys(changedData);
    pthread_mutex_lock(&watchListMutex);
    for (std::vector<NacosString>::iterator it = changedList.begin();
         it != changedList.end(); it++) {
        NacosString dataId, group, tenant;
        ParamUtils::parseString2KeyGroupTenant(*it, dataId, group, tenant);
        log_debug(
            "[ClientWorker]-performWatch:Processing item:%s, dataId = %s, "
            "group = %s, tenant = %s\n",
            it->c_str(), dataId.c_str(), group.c_str(), tenant.c_str());

        NacosString key = GroupKey::getKeyTenant(dataId, group, tenant);
        map<NacosString, ListeningData *>::iterator listenedDataIter =
            listeningKeys.find(key);
        // check whether the data being watched still exists
        if (listenedDataIter != listeningKeys.end()) {
            log_debug("[ClientWorker]-performWatch:Found entry for:%s\n",
                      key.c_str());
            ListeningData *listenedList = listenedDataIter->second;
            NacosString updatedcontent;

            try {
                updatedcontent = getServerConfig(
                    listenedList->getTenant(), listenedList->getDataId(),
                    listenedList->getGroup(),
                    _objectConfigData->_appConfigManager->getServeReqTimeout());
                md5.reset();
                md5.update(updatedcontent);
                listenedList->setMD5(md5.toString());
                log_debug(
                    "[ClientWorker]-performWatch:MD5 got for that data: %s\n",
                    listenedList->getMD5().c_str());
                const std::map<Listener *, char> *listenerList =
                    listenedList->getListenerList();
                for (std::map<Listener *, char>::const_iterator listenerIt =
                         listenerList->begin();
                     listenerIt != listenerList->end(); listenerIt++) {
                    Listener *curListener = listenerIt->first;
                    NACOS_ASSERT(curListener->refCnt() > 0);
                    curListener->receiveConfigInfo(updatedcontent);
                }
            } catch (NacosException &e) {
                // Same design as SubscriptionPoller
                log_warn(
                    "[ClientWorker]-performWatch:Encountered exception when "
                    "getting config from server:%s:%s:%s\n",
                    listenedList->getTenant().c_str(),
                    listenedList->getGroup().c_str(),
                    listenedList->getDataId().c_str());
                // wait for at lease 3 secs to avoid too many wake-ups when the
                // network is down
                sleep(MAX(_longPullingTimeout / 10000, 3));
                continue;
            }

            log_debug(
                "[ClientWorker]-performWatch:Data fetched from the server: "
                "%s\n",
                updatedcontent.c_str());
        }
    }
    clearDeleteList(20);  // TODO:constant
    pthread_mutex_unlock(&watchListMutex);
}

/**
 * Removes listeners in deleteList
 *
 * *NOT THREAD SAFE*, must be called within a watchListMutex guarded area or
 * after the watcher thread is stopped
 * @param maxRemoves if < 0, clear all items in the deleteList
 * @author Liu, Hanyu
 */
void ClientWorker::clearDeleteList(int maxRemoves) {
    int removeCount = 0;
    while (!deleteList.empty()) {
        // remove limited items every time
        if (maxRemoves > 0 && removeCount >= maxRemoves) {
            break;
        }
        std::list<OperateItem>::iterator it = deleteList.begin();
        OperateItem itm = *it;
        NacosString key = GroupKey::getKeyTenant(
            itm.getDataId(), itm.getGroup(), itm.getTenant());

        if (listeningKeys.find(key) == listeningKeys.end()) {
            log_warn(
                "[ClientWorker]-clearDeleteList:Trying to remove non-existent "
                "key: %s\n",
                key.c_str());
            deleteList.erase(it);
            continue;
        }

        ListeningData *slotOfListener = listeningKeys[key];

        Listener *theListener = itm.getListener();
        slotOfListener->removeListener(theListener);
        int refcount = theListener->decRef();
        log_debug(
            "[ClientWorker]-clearDeleteList:The listener (Name = %s) on "
            "deleteList is removed, key = %s.\n",
            theListener->getListenerName().c_str(), key.c_str());
        if (refcount == 0) {
            delete theListener;
        }
        itm.setListener(NULL);
        if (slotOfListener->isEmpty()) {
            log_debug(
                "[ClientWorker]-clearDeleteList:The slot (Name = %s) is empty "
                "and removed\n",
                key.c_str());
            delete slotOfListener;
            slotOfListener = NULL;
            listeningKeys.erase(key);
        }
        deleteList.erase(it);
        removeCount++;
    }
}

void ClientWorker::cleanUp() {
    log_debug("[ClientWorker]-cleanUp:entered\n");
    clearDeleteList(0);
    for (map<NacosString, ListeningData *>::iterator it = listeningKeys.begin();
         it != listeningKeys.end(); it++) {
        ListeningData *listeningData = it->second;
        log_debug("[ClientWorker]-cleanUp:Cleaning %s\n",
                  listeningData->toString().c_str());
        listeningData->clearListeners();
        delete listeningData;
        listeningData = NULL;
    }
}

void ClientWorker::addDeleteItem(const OperateItem &operateItem) {
    log_debug("[ClientWorker]-addDeleteItem:Adding delete item: %s\n",
              operateItem.toString().c_str());
    pthread_mutex_lock(&watchListMutex);
    deleteList.push_back(operateItem);
    pthread_mutex_unlock(&watchListMutex);
}

void ClientWorker::saveConfigToLocalFile(const NacosString &tenant,
                                         const NacosString &dataId,
                                         const NacosString &group,
                                         const HttpResult &httpResult)
    NACOS_THROW(NacosException) {
    AppConfigManager *_appConfigManager = _objectConfigData->_appConfigManager;
    LocalSnapshotManager *localSnapshotManager =
        _objectConfigData->_localSnapshotManager;
    switch (httpResult.code) {
        case HttpStatus::HTTP_OK:
            localSnapshotManager->saveSnapshot(
                _appConfigManager->get(PropertyKeyConst::CLIENT_NAME), dataId,
                group, tenant, httpResult.content);
            break;
        case HttpStatus::HTTP_NOT_FOUND:
            // Update snapshot
            localSnapshotManager->saveSnapshot(
                _appConfigManager->get(PropertyKeyConst::CLIENT_NAME), dataId,
                group, tenant, NULLSTR);
            throw NacosException(
                NacosException::HTTP_NOT_FOUND,
                "getServerConfig could not get content for Key " + group + ":" +
                    dataId);
            break;
        case HttpStatus::HTTP_FORBIDDEN:
            // Update snapshot
            localSnapshotManager->saveSnapshot(
                _appConfigManager->get(PropertyKeyConst::CLIENT_NAME), dataId,
                group, tenant, NULLSTR);
            throw NacosException(
                NacosException::NO_RIGHT,
                "permission denied for Key " + group + ":" + dataId);
            break;
        default:
            localSnapshotManager->saveSnapshot(
                _appConfigManager->get(PropertyKeyConst::CLIENT_NAME), dataId,
                group, tenant, NULLSTR);
            throw NacosException(NacosException::SERVER_ERROR,
                                 "getServerConfig failed with code:" +
                                     NacosStringOps::valueOf(httpResult.code));
            break;
    }
}

}  // namespace nacos