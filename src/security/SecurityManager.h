//
// Created by liuhanyu on 2020/11/28.
//

#ifndef NACOS_SDK_CPP_SECURITYMANAGER_H
#define NACOS_SDK_CPP_SECURITYMANAGER_H

#include "src/config/AppConfigManager.h"
#include "src/http/IHttpCli.h"
#include "src/server/ServerListManager.h"
#include "src/factory/ObjectConfigData.h"

namespace nacos {

struct AccessToken {
    NacosString accessToken;
    long tokenTtl;
    bool globalAdmin;
    long lastRefTime;
};

class SecurityManager {
private:
    ObjectConfigData *_objectConfigData;
    AccessToken _accessToken;
    void doLogin(const NacosString &serverAddr) throw(NacosException, NetworkException);
    void login() throw (NacosException);
    RWLock _rwLock;
public:
    SecurityManager(ObjectConfigData *objectConfigData);
    NacosString &getAccessToken();
};
}

#endif //NACOS_SDK_CPP_SECURITYMANAGER_H
