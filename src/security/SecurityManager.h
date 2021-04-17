//
// Created by liuhanyu on 2020/11/28.
//

#ifndef NACOS_SDK_CPP_SECURITYMANAGER_H
#define NACOS_SDK_CPP_SECURITYMANAGER_H

#include "src/config/AppConfigManager.h"
#include "src/http/IHttpCli.h"
#include "src/server/ServerListManager.h"
#include "src/factory/ObjectConfigData.h"
#include "Compatibility.h"

namespace nacos {

struct AccessToken {
    NacosString accessToken;
    long tokenTtl;
    bool globalAdmin;
    long lastRefTime;
    AccessToken() {
        accessToken = "";
        tokenTtl = 0;
        globalAdmin = false;
        lastRefTime = 0;
    }
};

class SecurityManager {
private:
    ObjectConfigData *_objectConfigData;
    AccessToken _accessToken;
    void doLogin(const NacosString &serverAddr) NACOS_THROW(NacosException, NetworkException);
    RWLock _rwLock;
    volatile bool _started;
    static void * tokenRefreshThreadFunc(void *param);
    Thread *_tokenRefreshThread;
    void sleepWithRunStatusCheck(long _milliSecsToSleep);
public:
    SecurityManager(ObjectConfigData *objectConfigData);
    ~SecurityManager();
    void login() NACOS_THROW (NacosException);
    NacosString &getAccessToken();
    void addAccessToken2Req(std::list<NacosString> &parameter);
    void start();
    void stop();
};
}

#endif //NACOS_SDK_CPP_SECURITYMANAGER_H
