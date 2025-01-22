//
// Created by liuhanyu on 2020/11/28.
//

#include "SecurityManager.h"
#include "src/json/JSON.h"
#include "src/utils/RandomUtils.h"
#include "src/utils/TimeUtils.h"
#include "src/utils/ParamUtils.h"
#include "constant/ConfigConstant.h"

using namespace std;
namespace nacos {
SecurityManager::SecurityManager(ObjectConfigData *objectConfigData) {
    _objectConfigData = objectConfigData;
    _started = false;
    _tokenRefreshThread = new Thread("TokenRefreshThread", tokenRefreshThreadFunc, (void*)this);
}
void SecurityManager::doLogin(const NacosString &serverAddr) NACOS_THROW(NacosException, NetworkException) {
    //TODO:refactor string constants
    NacosString url = serverAddr + "/" + _objectConfigData->_appConfigManager->getContextPath() + "/v1/auth/users/login";
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

void SecurityManager::login() NACOS_THROW (NacosException) {
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
        const NacosServerInfo &curServer = ParamUtils::getNthElem(serversToTry, (nr_tries + start) % nr_servers);
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
                /**
                 * Here we don't need to keep log for it, because there are 3 situations where we will call this login() routine:
                 * 1. Initialization stage of NamingService
                 * 2. Initialization stage of ConfigService
                 * 3. tokenRefreshThreadFunc's invocation of this routine to refresh the credentials
                 * In case 1 and case 2, the program will crash, because these situations are considered as a config error of the program, so let it crash
                 * In case 3, the log is printed by tokenRefreshThreadFunc to remind the dev-ops to correct the config
                */
                log_error("error INVALID_LOGIN_CREDENTIAL exit!");
                throw e;
            }
            log_error("Unknown error while login to server, e:%d = %s\n", e.errorcode(), e.what());
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
    if (_milliSecsToSleep == 0) {
        return;
    }
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
            log_debug("Ttl got from nacos server:%ld\n", thisObj->_accessToken.tokenTtl);
            thisObj->sleepWithRunStatusCheck(thisObj->_accessToken.tokenTtl * 1000);
            log_debug("Trying to login...\n");
            thisObj->login();
        } catch (NacosException &e) {
            if (e.errorcode() == NacosException::INVALID_LOGIN_CREDENTIAL) {
                /**
                 * invalid credential while the application is running, wait for a moment and retry
                 * since the existing data in the nacos client is still usable for the application
                 * we should keep the application alive for the Availability, but the Consistency, in this case, is NOT guaranteed
                 * the error log reminds the dev-ops to check the config
                 */
                log_error("Invalid credential, please check your server settings!\n");
                sleep(30);
            } else if (e.errorcode() == NacosException::ALL_SERVERS_TRIED_AND_FAILED) {
                log_warn("Network down, sleep for 30 secs and retry\n");
                sleep(30);//network down, wait for a moment
                continue;
            } else {
                //unknown error, there should be a better way to handle this situation
                log_error("Unknown error happend, code: %d, reason: %s\n", e.errorcode(), e.what());
                sleep(30);//unknown error, wait for 30 sec and continue
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
    _tokenRefreshThread->kill();
    _tokenRefreshThread->join();
}
}//nacos