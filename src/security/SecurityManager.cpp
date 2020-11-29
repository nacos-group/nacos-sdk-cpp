//
// Created by liuhanyu on 2020/11/28.
//

#include "SecurityManager.h"
#include "Parameters.h"
#include "src/json/JSON.h"
#include "utils/RandomUtils.h"

using namespace std;
namespace nacos {
SecurityManager::SecurityManager(
    AppConfigManager *appConfigManager,
    ServerListManager *serverListManager,
    IHttpCli *iHttpCli
) {
    _appConfigManager = appConfigManager;
    _serverListManager = serverListManager;
    _httpCli = iHttpCli;
}
void SecurityManager::doLogin(const NacosString &serverAddr) throw(NacosException, NetworkException) {
    //TODO:refactor string constants
    NacosString url = serverAddr + "/" + DEFAULT_CONTEXT_PATH + "/v1/auth/users/login";
    list <NacosString> headers;
    list <NacosString> paramValues;

    HttpResult result = _httpCli->httpPost(url, headers, paramValues, NULLSTR, 3000);

    _accessToken = JSON::Json2AccessToken(result.content);
}

void SecurityManager::login() throw (NacosException) {
    WriteGuard writeGuard(_rwLock);
    list <NacosServerInfo> serversToTry = _serverListManager->getServerList();
    size_t nr_servers = serversToTry.size();
    if (nr_servers == 0) {
        throw NacosException(NacosException::NO_SERVER_AVAILABLE, "No available server when getting access token");
    }

    size_t start = RandomUtils::random(0, nr_servers - 1);
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
}//nacos