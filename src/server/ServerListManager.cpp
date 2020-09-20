#include <stdlib.h>
#include <unistd.h>
#include "server/ServerListManager.h"
#include "PropertyKeyConst.h"
#include "Parameters.h"
#include "utils/ParamUtils.h"
#include "Debug.h"
#include "json/JSON.h"

using namespace std;

void ServerListManager::addToSrvList(NacosString &address) {
    //If the address doesn't contain port, add 8848 as the default port for it
    if (address.find(':') == std::string::npos) {
        NacosServerInfo curServer;
        curServer.setKey(address + ":8848");
        curServer.setAlive(true);
        curServer.setIp(address);
        //TODO:dynamically read default port, don't use hard-coded value
        curServer.setPort(8848);
        curServer.setWeight(1.0);
        curServer.setAdWeight(1.0);
        serverList.push_back(curServer);
    } else {
        vector <NacosString> explodedAddress;
        ParamUtils::Explode(explodedAddress, address, ':');
        NacosServerInfo curServer;
        curServer.setKey(address);
        curServer.setAlive(true);
        curServer.setIp(explodedAddress[0]);
        curServer.setPort(atoi(explodedAddress[1].c_str()));
        curServer.setWeight(1.0);
        curServer.setAdWeight(1.0);
        serverList.push_back(curServer);
    }
}

ServerListManager::ServerListManager(list <NacosString> &fixed) {
    started = false;
    isFixed = true;
    refreshInterval = 30000;
    _read_timeout = 3000;
    for (list<NacosString>::iterator it = fixed.begin(); it != fixed.end(); it++) {
        addToSrvList(*it);
    }
}

NacosString ServerListManager::getCurrentServerAddr() {
    //By default, this function returns a server in the serverList randomly
    //later we should sort it according to the java client and use cache
    ReadGuard _readGuard(rwLock);
    size_t max_serv_slot = serverList.size();
    srand(time(NULL));
    int to_skip = rand() % max_serv_slot;
    std::list<NacosServerInfo>::iterator it = serverList.begin();
    for (int skipper = 0; skipper < to_skip; skipper++) {
        it++;
    }

    return it->getCompleteAddress();
}

void ServerListManager::initAll() throw(NacosException) {
    serverList.clear();
    Properties props = appConfigManager->getAllConfig();
    if (props.contains(PropertyKeyConst::SERVER_ADDR)) {//Server address is configured
        isFixed = true;
        NacosString server_addr = props[PropertyKeyConst::SERVER_ADDR];

        vector <NacosString> explodedServers;
        ParamUtils::Explode(explodedServers, server_addr, ',');
        for (vector<NacosString>::iterator it = explodedServers.begin(); it != explodedServers.end(); it++) {
            addToSrvList(*it);
        }
        serverList.sort();
    } else {//use endpoint mode to pull nacos server info from server
        if (!props.contains(PropertyKeyConst::ENDPOINT)) {
            throw NacosException(NacosException::CLIENT_INVALID_PARAM, "endpoint is blank");
        }

        isFixed = false;
        if (NacosStringOps::isNullStr(getNamespace())) {
            addressServerUrl = getEndpoint() + ":" + NacosStringOps::valueOf(getEndpointPort()) + "/" +
                               getContextPath() + "/" + getClusterName();
        } else {
            addressServerUrl = getEndpoint() + ":" + NacosStringOps::valueOf(getEndpointPort()) + "/" +
                               getContextPath() + "/" + getClusterName() + "?namespace=" + getNamespace();
        }

        log_debug("Assembled addressServerUrl:%s\n", addressServerUrl.c_str());

        serverList = pullServerList();
        start();
    }
}

ServerListManager::ServerListManager(HTTPCli *_httpCli, AppConfigManager *_appConfigManager) throw(NacosException) {
    started = false;
    this->httpCli = _httpCli;
    this->appConfigManager = _appConfigManager;
    refreshInterval = atoi(appConfigManager->get(PropertyKeyConst::SRVLISTMGR_REFRESH_INTERVAL).c_str());
    _read_timeout = atoi(appConfigManager->get(PropertyKeyConst::SRVLISTMGR_READ_TIMEOUT).c_str());
    initAll();
}

list <NacosServerInfo> ServerListManager::tryPullServerListFromNacosServer() throw(NacosException) {
    std::list <NacosString> headers;
    std::list <NacosString> paramValues;
    size_t maxSvrSlot = serverList.size();
    if (maxSvrSlot == 0) {
        throw NacosException(0, "failed to get nacos servers, raison: no server(s) available");
    }
    log_debug("nr_servers:%d\n", maxSvrSlot);
    srand(time(NULL));

    NacosString errmsg;
    for (size_t i = 0; i < serverList.size(); i++) {
        size_t selectedServer = rand() % maxSvrSlot;
        NacosServerInfo server = ParamUtils::getNthElem(serverList, selectedServer);
        log_debug("selected_server:%d\n", selectedServer);
        log_debug("Trying to access server:%s\n", server.getCompleteAddress().c_str());
        try {
            HttpResult serverRes = httpCli->httpGet(
                    server.getCompleteAddress() + "/" + DEFAULT_CONTEXT_PATH + "/" + PROTOCOL_VERSION + "/" +
                    GET_SERVERS_PATH,
                    headers, paramValues, NULLSTR, _read_timeout);
            return JSON::Json2NacosServerInfo(serverRes.content);
        }
        catch (NacosException &e) {
            errmsg = e.what();
            log_error("request %s failed.\n", server.getCompleteAddress().c_str());
        }
        catch (exception &e) {
            errmsg = e.what();
            log_error("request %s failed.\n", server.getCompleteAddress().c_str());
        }

        selectedServer = (selectedServer + 1) % serverList.size();
    }

    throw NacosException(0,
                         "failed to get nacos servers after all servers(" + toString() + ") tried: "
                         + errmsg);
}

list <NacosServerInfo> ServerListManager::pullServerList() throw(NacosException) {
    std::list <NacosString> headers;
    std::list <NacosString> paramValues;

    if (!NacosStringOps::isNullStr(addressServerUrl)) {
        HttpResult serverRes = httpCli->httpGet(addressServerUrl, headers, paramValues, NULLSTR,
                                                _read_timeout);
        list <NacosServerInfo> serversPulled = JSON::Json2NacosServerInfo(serverRes.content);
        serversPulled.sort();

        log_debug("pullServerList: servers list: %s\n", serverListToString(serversPulled).c_str());
        return serversPulled;
    }
    //usually this should not be happening
    throw NacosException(0, "addressServerUrl is not set, please config it on properties");
}

std::list <NacosServerInfo> ServerListManager::__debug() {
    return tryPullServerListFromNacosServer();
}

NacosString ServerListManager::serverListToString(const std::list <NacosServerInfo> &serverList) {
    NacosString res;
    bool first = true;
    for (list<NacosServerInfo>::const_iterator it = serverList.begin(); it != serverList.end(); it++) {
        if (first) {
            first = false;
        } else {
            res += ",";
        }
        res += it->toString();
    }

    return res;
}

NacosString ServerListManager::toString() const {
    return serverListToString(serverList);
}

void *ServerListManager::pullWorkerThread(void *param) {
    ServerListManager *thisMgr = (ServerListManager *) param;
    while (thisMgr->started) {
        try {
            bool changed = false;
            list <NacosServerInfo> serverList = thisMgr->pullServerList();

            {
                ReadGuard _readGuard(thisMgr->rwLock);
                if (serverList != thisMgr->serverList) {
                    log_debug("Servers got from nacos differs from local one, updating...\n");
                    changed = true;
                }
            }

            if (changed) {
                WriteGuard _writeGuard(thisMgr->rwLock);
                log_debug("updated!\n");
                thisMgr->serverList = serverList;
            }
        }
        catch (NacosException e) {
            //Error occured during the invocation, sleep for a longer time
            sleep(thisMgr->refreshInterval / 1000);
        }
        sleep(thisMgr->refreshInterval / 1000);
    }

    return NULL;
}

void ServerListManager::start() {
    if (started && !isFixed) {
        return;
    }

    started = true;

    if (_pullThread == NULL) {
        NacosString threadName = getClusterName() + "," + getEndpoint() + ":" +
                                 NacosStringOps::valueOf(getEndpointPort()) + "-" + getNamespace();
        _pullThread = new Thread(threadName, pullWorkerThread, (void *) this);
    }
    _pullThread->start();
}

void ServerListManager::stop() {
    if (!started) {
        return;
    }

    started = false;
    if (_pullThread != NULL) {
        _pullThread->join();
    }
}

NacosString ServerListManager::getContextPath() const {
    return appConfigManager->get(PropertyKeyConst::CONTEXT_PATH);
}

ServerListManager::~ServerListManager() {
    stop();
    if (_pullThread != NULL) {
        delete _pullThread;
        _pullThread = NULL;
    }
}

int ServerListManager::getServerCount() {
    int count = 0;
    {
        ReadGuard _readGuard(rwLock);
        count = serverList.size();
    }
    return count;
};

list <NacosServerInfo> ServerListManager::getServerList() {
    std::list <NacosServerInfo> res;
    {
        ReadGuard _readGuard(rwLock);
        res = serverList;
    }
    return res;
};