//
// Created by liuhanyu on 2020/11/28.
//

#include "SecurityManager.h"
#include "src/json/JSON.h"
#include "utils/RandomUtils.h"
#include "src/utils/TimeUtils.h"

using namespace std;
namespace nacos {
SecurityManager::SecurityManager(ObjectConfigData *objectConfigData) {
    _objectConfigData = objectConfigData;
    _started = false;
    _tokenRefreshThread = new Thread("TokenRefreshThread", tokenRefreshThreadFunc, (void*)this);
}
void SecurityManager::doLogin(const NacosString &serverAddr) throw(NacosException, NetworkException) {
    //TODO:refactor string constants
    NacosString url = serverAddr + "/" + Constants::DEFAULT_CONTEXT_PATH + "/v1/auth/users/login";
    list <NacosString> headers;
    list <NacosString> paramValues;

    const NacosString &username = _objectConfigData->_appConfigManager->get(PropertyKeyConst::AUTH_USERNAME);
    const NacosString &password = _objectConfigData->_appConfigManager->get(PropertyKeyConst::AUTH_PASSWORD);

    paramValues.push_back("username");
    paramValues.push_back(username);
    paramValues.push_back("password");
    paramValues.push_back(password);

    HttpResult result = _objectConfigData->_httpCli->httpPost(url, headers, paramValues, NULLSTR, 3000);

    _accessToken = JSON::Json2AccessToken(result.content);
}

void SecurityManager::login() throw (NacosException) {
    WriteGuard writeGuard(_rwLock);
    list <NacosServerInfo> serversToTry = _objectConfigData->_serverListManager->getServerList();
    size_t nr_servers = serversToTry.size();
    if (nr_servers == 0) {
        throw NacosException(NacosException::NO_SERVER_AVAILABLE, "No available server when getting access token");
    }

    size_t start = 0;

    if (nr_servers > 1) {
        start = RandomUtils::random(0, nr_servers - 1);
    }

    for (size_t nr_tries = 0; nr_tries < nr_servers; nr_tries++) {
        const NacosServerInfo &curServer = ParamUtils::getNthElem(serversToTry, nr_tries + start % nr_servers);
        NacosString serverAddr = curServer.getCompleteAddress();
        try {
            //the method will throw if there's something wrong(e.g.: network problem)
            doLogin(serverAddr);
        } catch (NetworkException &e) {
            //continue to try next node
            continue;
        } catch (NacosException &e) {
            //for some cases, e.g.:invalid username/password,
            //we should throw exception directly since retry on another node will not correct this problem
            if (e.errorcode() == NacosException::INVALID_LOGIN_CREDENTIAL) {
                throw e;
            }
            continue;
        }
        //login succeeded
        return;
    }
    //this is (usually) a network problem, the caller (thread) should handle this
    throw NacosException(NacosException::ALL_SERVERS_TRIED_AND_FAILED, "Login failed after all servers are tried");
}

NacosString &SecurityManager::getAccessToken() {
    ReadGuard _readGuard(_rwLock);
    return _accessToken.accessToken;
}

void SecurityManager::addAccessToken2Req(std::list<NacosString> &parameter){
    ReadGuard _readGuard(_rwLock);
    parameter.push_back("accessToken");
    parameter.push_back(_accessToken.accessToken);
}

SecurityManager::~SecurityManager() {
    stop();
    delete _tokenRefreshThread;
    _tokenRefreshThread = NULL;
}

void SecurityManager::sleepWithRunStatusCheck(long _milliSecsToSleep) {
    long granularity = 10;
    long sleep_start_time = TimeUtils::getCurrentTimeInMs();
    long sleep_end_time = sleep_start_time + _milliSecsToSleep;
    while (_started) {
        if (TimeUtils::getCurrentTimeInMs() >= sleep_end_time) {
            break;
        }
        sleep(granularity);
    }
}

void *SecurityManager::tokenRefreshThreadFunc(void *param) {
    SecurityManager *thisObj = (SecurityManager*)param;
    log_debug("In thread SecurityManager::tokenRefreshThreadFunc\n");
    while (thisObj->_started) {
        try {
            log_debug("Trying to login...\n");
            thisObj->login();
            log_debug("Ttl got from nacos server:%ld\n", thisObj->_accessToken.tokenTtl);
            thisObj->sleepWithRunStatusCheck(thisObj->_accessToken.tokenTtl);
        } catch (NacosException &e) {
            if (e.errorcode() == NacosException::INVALID_LOGIN_CREDENTIAL) {
                throw e;//Invalid login credential, let it crash
            } else if (e.errorcode() == NacosException::ALL_SERVERS_TRIED_AND_FAILED) {
                sleep(30);//network down, wait for a moment
                continue;
            }
        }
    }
    return NULL;
}

void SecurityManager::start() {
    if (_started) {
        return;
    }

    _started = true;
    _tokenRefreshThread->start();
}

void SecurityManager::stop() {
    if (!_started) {
        return;
    }
    _started = false;
    _tokenRefreshThread->join();
}
}//nacos