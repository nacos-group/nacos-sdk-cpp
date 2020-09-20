#ifndef __NACOS_CFG_SVC_H_
#define __NACOS_CFG_SVC_H_

#include "config/ConfigService.h"
#include "http/HttpAgent.h"
#include "listen/ClientWorker.h"
#include "NacosString.h"
#include "server/ServerListManager.h"
#include "Properties.h"

class NacosConfigService : public ConfigService {
private:
    HttpAgent *httpAgent = NULL;
    HTTPCli *httpcli = NULL;
    ServerListManager *svrListMgr = NULL;
    ClientWorker *clientWorker = NULL;
    AppConfigManager *appConfigManager = NULL;

    //Private Methods
    NacosConfigService();

    NacosString getConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group,
                               long timeoutMs) throw(NacosException);

    bool removeConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group,
                           const NacosString &tag) throw(NacosException);

    bool publishConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group,
                            const NacosString &tag, const NacosString &appName, const NacosString &betaIps,
                            const NacosString &content) throw(NacosException);
    //NacosString monitorChange(std::map<NacosString, NacosString> &keysAndContents, long timeoutMs) throw (NacosException);
    //static NacosString monitorListToString(std::map<NacosString, NacosString> &keysAndContents);

    NacosString getNamespace() const { return appConfigManager->get(PropertyKeyConst::NAMESPACE); };
public:
    const static long POST_TIMEOUT = 3000L;

    //Public Methods
    NacosString getConfig(const NacosString &dataId, const NacosString &group, long timeoutMs) throw(NacosException);

    bool publishConfig(const NacosString &dataId, const NacosString &group,
                       const NacosString &content) throw(NacosException);

    bool removeConfig(const NacosString &dataId, const NacosString &group) throw(NacosException);

    void addListener(const NacosString &dataId, const NacosString &group, Listener *listener) throw(NacosException);

    void removeListener(const NacosString &dataId, const NacosString &group, Listener *listener);

    HttpAgent *getHttpAgent() const { return httpAgent; };

    HTTPCli *getHttpCli() const { return httpcli; };

    ServerListManager *getServerListManager() const { return svrListMgr; };

    ClientWorker *getClientWorker() const { return clientWorker; };

    AppConfigManager *getAppConfigManager() const { return appConfigManager; };

    void setHttpAgent(HttpAgent *_httpAgent) { httpAgent = _httpAgent; };

    void setHttpCli(HTTPCli *_httpCli) { httpcli = _httpCli; };

    void setServerListManager(ServerListManager *_svrListMgr) { svrListMgr = _svrListMgr; };

    void setClientWorker(ClientWorker *_clientWorker) { clientWorker = _clientWorker; };

    void setAppConfigManager(AppConfigManager *_appConfigManager) { appConfigManager = _appConfigManager; };

    //ctors/dtor
    NacosConfigService(AppConfigManager *_appConfigManager,
                       HTTPCli *_httpCli,
                       ServerListManager *_serverListManager,
                       HttpAgent *_httpAgent,
                       ClientWorker *_clientWorker) throw(NacosException);

    virtual ~NacosConfigService();
};

#endif
