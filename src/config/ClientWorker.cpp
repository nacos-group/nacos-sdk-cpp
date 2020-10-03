#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include "listen/ClientWorker.h"
#include "listen/Listener.h"
#include "utils/url.h"
#include "utils/GroupKey.h"
#include "http/httpStatCode.h"
#include "md5/md5.h"
#include "utils/ParamUtils.h"
#include "Debug.h"
#include "DebugAssertion.h"
#include "Constants.h"
#include "Parameters.h"
#include "PropertyKeyConst.h"

using namespace std;

ClientWorker::ClientWorker(HttpAgent *_httpAgent, AppConfigManager *_appConfigManager) {
    threadId = 0;
    stopThread = true;
    pthread_mutex_init(&watchListMutex, NULL);
    pthread_mutex_init(&stopThreadMutex, NULL);
    httpAgent = _httpAgent;
    appConfigManager = _appConfigManager;

    _longPullingTimeoutStr = appConfigManager->get(PropertyKeyConst::CONFIG_LONGPULLLING_TIMEOUT);
    _longPullingTimeout = atoi(_longPullingTimeoutStr.c_str());
    _readTimeout = atoi(appConfigManager->get(PropertyKeyConst::CONFIG_GET_TIMEOUT).c_str());
}

ClientWorker::~ClientWorker() {
    log_debug("ClientWorker::~ClientWorker()\n");
    stopListening();
    cleanUp();
}

int64_t getCurrentTimeInMs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

NacosString ClientWorker::getServerConfig
        (
                const NacosString &tenant,
                const NacosString &dataId,
                const NacosString &group,
                long timeoutMs
        ) throw(NacosException) {
    std::list <NacosString> headers;
    std::list <NacosString> paramValues;
    //Get the request url
    NacosString url = DEFAULT_CONTEXT_PATH + Constants::CONFIG_CONTROLLER_PATH;

    HttpResult res;

    paramValues.push_back("dataId");
    paramValues.push_back(dataId);
    if (!isNull(group)) {
        paramValues.push_back("group");
        paramValues.push_back(group);
    } else {
        paramValues.push_back("group");
        paramValues.push_back(Constants::DEFAULT_GROUP);
    }

    if (!isNull(tenant)) {
        paramValues.push_back("tenant");
        paramValues.push_back(tenant);
    }

    try {
        res = httpAgent->httpGet(url, headers, paramValues, httpAgent->getEncode(), timeoutMs);
    }
    catch (NetworkException &e) {
        throw NacosException(NacosException::SERVER_ERROR, e.what());
    }

    switch (res.code) {
        case HTTP_OK:
            return res.content;
        case HTTP_NOT_FOUND:
            return NULLSTR;
    }
    return NULLSTR;
}

void *ClientWorker::listenerThread(void *parm) {
    log_debug("Entered watch thread...\n");
    ClientWorker *thelistener = (ClientWorker *) parm;

    while (!thelistener->stopThread) {
        int64_t start_time = getCurrentTimeInMs();
        log_debug("Start watching at %u...\n", start_time);
        thelistener->performWatch();

        log_debug("Watch function exit at %u...\n", getCurrentTimeInMs());
    }

    return 0;
}

vector <NacosString> ClientWorker::parseListenedKeys(const NacosString &ReturnedKeys) {
    NacosString changedKeyList = urldecode(ReturnedKeys);

    vector <NacosString> explodedList;
    ParamUtils::Explode(explodedList, changedKeyList, Constants::LINE_SEPARATOR);

    //If the server returns a string with a trailing \x01, actually there is no data after that
    //but ParamUtils::Explode will return an extra item with empty string, we need to remove that
    //from the list so it won't disrupt subsequent operations
    log_debug("extra data:%s\n", explodedList[explodedList.size() - 1].c_str());
    if (explodedList.size() >= 1 && ParamUtils::isBlank(explodedList[explodedList.size() - 1])) {
        explodedList.pop_back();
    }
    return explodedList;
}

void ClientWorker::startListening() {
    //Already started, skip this
    if (!stopThread) {
        log_debug("The thread is already started or the starting is in progress...\n");
        return;
    }

    pthread_mutex_lock(&stopThreadMutex);
    if (!stopThread) {
        pthread_mutex_unlock(&stopThreadMutex);
        log_debug("The thread is already started or the starting is in progress...\n");
        return;
    }

    stopThread = false;
    pthread_mutex_unlock(&stopThreadMutex);

    log_debug("Starting the thread...\n");
    pthread_create(&threadId, NULL, listenerThread, (void *) this);
    log_debug("Started thread with id:%d...\n", threadId);
}

void ClientWorker::stopListening() {
    log_debug("ClientWorker::stopListening()\n");
    if (stopThread)//Stop in progress
    {
        log_debug("The thread is already stopped or the stop is in progress...\n");
        return;
    }

    pthread_mutex_lock(&stopThreadMutex);
    if (stopThread)//Stop in progress
    {
        pthread_mutex_unlock(&stopThreadMutex);
        log_debug("The thread is already stopped or the stop is in progress...\n");
        return;
    }

    stopThread = true;
    pthread_mutex_unlock(&stopThreadMutex);

    pthread_join(threadId, NULL);
    log_info("The thread is stopped successfully...\n");

}

void ClientWorker::addListener
        (
                const NacosString &dataId,
                const NacosString &group,
                const NacosString &tenant,
                const NacosString &initialContent,
                Listener *listener
        ) {
    NacosString key = GroupKey::getKeyTenant(dataId, group, tenant);
    log_debug("Adding listener with key: %s\n", key.c_str());
    pthread_mutex_lock(&watchListMutex);

    //Check whether the listener being added to the list already exists
    if (listeningKeys.find(key) != listeningKeys.end()) {
        ListeningData *curListeningData = listeningKeys[key];
        if (!curListeningData->addListener(listener)) {
            log_warn("Key %s is already in the watch list, leaving...\n", key.c_str());
        }
        listener->incRef();
        pthread_mutex_unlock(&watchListMutex);
        return;
    }

    //if the listener does not exist, just create it

    ListeningData *listeningData = new ListeningData(tenant, dataId, group, initialContent);

    //If no, copy one
    listeningKeys[key] = listeningData;
    listeningData->addListener(listener);
    listener->incRef();
    pthread_mutex_unlock(&watchListMutex);
    log_debug("Key %s is added successfully!\n", key.c_str());
}

/**
* Removes a listener from the listened list actively, but slow
* @param listener the listener to be removed
* @author Liu, Hanyu
*/
void ClientWorker::removeListenerActively
        (
                const NacosString &dataId,
                const NacosString &group,
                const NacosString &tenant,
                Listener *listener
        ) {
    NacosString key = GroupKey::getKeyTenant(dataId, group, tenant);
    pthread_mutex_lock(&watchListMutex);
    map<NacosString, ListeningData *>::iterator it = listeningKeys.find(key);
    //Check whether the cachedata being removed exists
    if (it == listeningKeys.end()) {
        log_warn("Removing a non-existing listener %s, leaving...\n", key.c_str());
        pthread_mutex_unlock(&watchListMutex);
        return;
    }

    //If so, remove it and free the resources
    ListeningData *curListeningData = it->second;
    bool succRemoved = curListeningData->removeListener(listener);
    if (!succRemoved) {
        log_warn("Removing a non-existing listener %s...\n", key.c_str());
    } else {
        //remove the listener, it is created by the client but freed by nacos-sdk-cpp
        log_debug("Removing a listener %s...\n", key.c_str());
        int refcount = listener->decRef();
        if (refcount == 0) {
            log_debug("Refcount of the listener(Name = %s) is 0 so delete it.\n", listener->getListenerName().c_str());
            delete listener;
            listener = NULL;
        }
    }

    //no listener on the list, remove the entry
    if (curListeningData->isEmpty()) {
        listeningKeys.erase(key);
        //free the space for this slot
        delete curListeningData;
    }
    pthread_mutex_unlock(&watchListMutex);
}

/**
* Removes a listener from the listened list(very fast compared to removeListenerActively)
* This function will return fast,
* since it just mark the remove flag of the listener and wait for the watcher to actually remove it
* @param listener the listener to be removed
* @author Liu, Hanyu
*/
void ClientWorker::removeListener
        (
                const NacosString &dataId,
                const NacosString &group,
                const NacosString &tenant,
                Listener *listener
        ) {
    //set the remove flag and return quickly
    //the background process will remove the listener
    //add this listener to the remove list since it is marked to be removed
    OperateItem operateItem(dataId, tenant, group, listener);
    addDeleteItem(operateItem);
}

NacosString ClientWorker::checkListenedKeys() {
    NacosString postData;
    pthread_mutex_lock(&watchListMutex);
    for (map<NacosString, ListeningData *>::iterator it = listeningKeys.begin(); it != listeningKeys.end(); it++) {
        ListeningData *curListenedKey = it->second;

        postData += curListenedKey->getDataId();
        postData += Constants::WORD_SEPARATOR;
        postData += curListenedKey->getGroup();
        postData += Constants::WORD_SEPARATOR;

        if (!isNull(curListenedKey->getTenant())) {
            postData += curListenedKey->getMD5();
            postData += Constants::WORD_SEPARATOR;
            postData += curListenedKey->getTenant();
            postData += Constants::LINE_SEPARATOR;
        } else {
            postData += curListenedKey->getMD5();
            postData += Constants::LINE_SEPARATOR;
        }
    }
    pthread_mutex_unlock(&watchListMutex);

    list <NacosString> headers;
    list <NacosString> paramValues;

    headers.push_back("Long-Pulling-Timeout");
    headers.push_back(_longPullingTimeoutStr);

    paramValues.push_back(Constants::PROBE_MODIFY_REQUEST);
    paramValues.push_back(postData);
    log_debug("Assembled postData:%s\n", postData.c_str());

    //Get the request url
    //TODO:move /listener to constant
    NacosString url = DEFAULT_CONTEXT_PATH + Constants::CONFIG_CONTROLLER_PATH + "/listener";
    HttpResult res;

    try {
        res = httpAgent->httpPost(url, headers, paramValues, httpAgent->getEncode(), _readTimeout);
    }
    catch (NetworkException &e) {
        log_warn("Request failed with: %s\n", e.what());
        NacosString result = "";
        return result;
    }

    log_debug("Received the message below from server:\n%s\n", res.content.c_str());
    return res.content;
}

void ClientWorker::performWatch() {
    MD5 md5;
    NacosString changedData = checkListenedKeys();
    vector <NacosString> changedList = ClientWorker::parseListenedKeys(changedData);
    pthread_mutex_lock(&watchListMutex);
    for (std::vector<NacosString>::iterator it = changedList.begin(); it != changedList.end(); it++) {
        NacosString dataId, group, tenant;
        ParamUtils::parseString2KeyGroupTenant(*it, dataId, group, tenant);
        log_debug("Processing item:%s, dataId = %s, group = %s, tenant = %s\n",
                  it->c_str(), dataId.c_str(), group.c_str(), tenant.c_str());

        NacosString key = GroupKey::getKeyTenant(dataId, group, tenant);
        map<NacosString, ListeningData *>::iterator listenedDataIter = listeningKeys.find(key);
        //check whether the data being watched still exists
        if (listenedDataIter != listeningKeys.end()) {
            log_debug("Found entry for:%s\n", key.c_str());
            ListeningData *listenedList = listenedDataIter->second;
            NacosString updatedcontent = "";

            try {
                updatedcontent = getServerConfig(listenedList->getTenant(), listenedList->getDataId(),
                                                 listenedList->getGroup(), _readTimeout);
            }
            catch (NacosException &e) {
                //Same design as TcpNamingServicePoller
                log_warn("Encountered exception when getting config from server:%s:%s:%s\n",
                         listenedList->getTenant().c_str(),
                         listenedList->getGroup().c_str(),
                         listenedList->getDataId().c_str());
                sleep(_longPullingTimeout / 1000);
                break;
            }
            log_debug("Data fetched from the server: %s\n", updatedcontent.c_str());
            md5.reset();
            md5.update(updatedcontent.c_str());
            listenedList->setMD5(md5.toString());
            log_debug("MD5 got for that data: %s\n", listenedList->getMD5().c_str());
            std::map < Listener * , char > const *listenerList = listenedList->getListenerList();
            for (std::map<Listener *, char>::const_iterator listenerIt = listenerList->begin();
                 listenerIt != listenerList->end(); listenerIt++) {
                Listener *curListener = listenerIt->first;

                NACOS_ASSERT(curListener->refCnt() > 0);
                curListener->receiveConfigInfo(updatedcontent);
            }
        }
    }
    clearDeleteList(20);//TODO:constant
    pthread_mutex_unlock(&watchListMutex);
}

/**
* Removes listeners in deleteList
*
* *NOT THREAD SAFE*, must be called within a watchListMutex guarded area or after the watcher thread is stopped
* @param maxRemoves if < 0, clear all items in the deleteList
* @author Liu, Hanyu
*/
void ClientWorker::clearDeleteList(int maxRemoves) {
    int removeCount = 0;
    while (!deleteList.empty()) {
        //remove limited items every time
        if (maxRemoves > 0 && removeCount >= maxRemoves) {
            break;
        }
        std::list<OperateItem>::iterator it = deleteList.begin();
        OperateItem itm = *it;
        NacosString key = GroupKey::getKeyTenant(itm.getDataId(), itm.getGroup(), itm.getTenant());

        if (listeningKeys.find(key) == listeningKeys.end()) {
            log_warn("Trying to remove non-existent key: %s\n", key.c_str());
            deleteList.erase(it);
            continue;
        }

        ListeningData *slotOfListener = listeningKeys[key];

        Listener *theListener = itm.getListener();
        slotOfListener->removeListener(theListener);
        int refcount = theListener->decRef();
        log_debug("The listener (Name = %s) on deleteList is removed, key = %s.\n",
                  theListener->getListenerName().c_str(), key.c_str());
        if (refcount == 0) {
            delete theListener;
        }
        itm.setListener(NULL);
        if (slotOfListener->isEmpty()) {
            log_debug("The slot (Name = %s) is empty and removed\n", key.c_str());
            delete slotOfListener;
            slotOfListener = NULL;
            listeningKeys.erase(key);
        }
        deleteList.erase(it);
        removeCount++;
    }
}

void ClientWorker::cleanUp() {
    log_debug("ClientWorker::cleanUp()\n");
    clearDeleteList(0);
    for (map<NacosString, ListeningData *>::iterator it = listeningKeys.begin(); it != listeningKeys.end(); it++) {
        ListeningData *listeningData = it->second;
        log_debug("Cleaning %s\n", listeningData->toString().c_str());
        listeningData->clearListeners();
        delete listeningData;
        listeningData = NULL;
    }
}

void ClientWorker::addDeleteItem(const OperateItem &operateItem) {
    log_debug("Adding delete item: %s\n", operateItem.toString().c_str());
    pthread_mutex_lock(&watchListMutex);
    deleteList.push_back(operateItem);
    pthread_mutex_unlock(&watchListMutex);
}