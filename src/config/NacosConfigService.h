#ifndef __NACOS_CFG_SVC_H_
#define __NACOS_CFG_SVC_H_

#include "config/ConfigService.h"
#include "src/http/HttpDelegate.h"
#include "src/listen/ClientWorker.h"
#include "NacosString.h"
#include "src/server/ServerListManager.h"
#include "Properties.h"

namespace nacos{
class NacosConfigService : public ConfigService {
private:
    HttpDelegate *_httpDelegate = NULL;
    IHttpCli *_httpCli = NULL;
    ServerListManager *_svrListMgr = NULL;
    ClientWorker *_clientWorker = NULL;
    AppConfigManager *_appConfigManager = NULL;
    LocalSnapshotManager *_localSnapshotManager = NULL;

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

    NacosString getNamespace() const { return _appConfigManager->get(PropertyKeyConst::NAMESPACE); };
public:
    const static long POST_TIMEOUT = 3000L;

    //Public Methods
    NacosString getConfig(const NacosString &dataId, const NacosString &group, long timeoutMs) throw(NacosException);

    bool publishConfig(const NacosString &dataId, const NacosString &group,
                       const NacosString &content) throw(NacosException);

    bool removeConfig(const NacosString &dataId, const NacosString &group) throw(NacosException);

    void addListener(const NacosString &dataId, const NacosString &group, Listener *listener) throw(NacosException);

    void removeListener(const NacosString &dataId, const NacosString &group, Listener *listener);

    HttpDelegate *getHttpDelegate() const { return _httpDelegate; };

    IHttpCli *getHttpCli() const { return _httpCli; };

    ServerListManager *getServerListManager() const { return _svrListMgr; };

    ClientWorker *getClientWorker() const { return _clientWorker; };

    AppConfigManager *getAppConfigManager() const { return _appConfigManager; };

    void setHttpDelegate(HttpDelegate *httpDelegate) { _httpDelegate = httpDelegate; };

    void setHttpCli(IHttpCli *httpCli) { _httpCli = httpCli; };

    void setServerListManager(ServerListManager *svrListMgr) { _svrListMgr = svrListMgr; };

    void setClientWorker(ClientWorker *clientWorker) { _clientWorker = clientWorker; };

    void setAppConfigManager(AppConfigManager *appConfigManager) { _appConfigManager = appConfigManager; };

    //ctors/dtor
    NacosConfigService(AppConfigManager *_appConfigManager,
                       IHttpCli *httpCli,
                       HttpDelegate *httpDelegate,
                       ServerListManager *serverListManager,
                       ClientWorker *clientWorker,
                       LocalSnapshotManager *localSnapshotManager) throw(NacosException);

    virtual ~NacosConfigService();
};
}//namespace nacos

#endif
