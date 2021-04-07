#ifndef __NACOS_CFG_SVC_H_
#define __NACOS_CFG_SVC_H_

#include "config/ConfigService.h"
#include "src/http/HttpDelegate.h"
#include "src/listen/ClientWorker.h"
#include "NacosString.h"
#include "src/server/ServerListManager.h"
#include "Properties.h"
#include "src/factory/ObjectConfigData.h"
#include "Compatibility.h"

namespace nacos{
class NacosConfigService : public ConfigService {
private:
    ObjectConfigData *_objectConfigData;

    //Private Methods
    NacosConfigService();

    NacosString getConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group,
                               long timeoutMs) NACOS_THROW(NacosException);

    bool removeConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group,
                           const NacosString &tag) NACOS_THROW(NacosException);

    bool publishConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group,
                            const NacosString &tag, const NacosString &appName, const NacosString &betaIps,
                            const NacosString &content) NACOS_THROW(NacosException);
    //NacosString monitorChange(std::map<NacosString, NacosString> &keysAndContents, long timeoutMs) throw (NacosException);
    //static NacosString monitorListToString(std::map<NacosString, NacosString> &keysAndContents);

    NacosString getNamespace() const { return _objectConfigData->_appConfigManager->get(PropertyKeyConst::NAMESPACE); };
public:
    const static long POST_TIMEOUT = 3000L;

    //Public Methods
    NacosString getConfig(const NacosString &dataId, const NacosString &group, long timeoutMs) NACOS_THROW(NacosException);

    bool publishConfig(const NacosString &dataId, const NacosString &group,
                       const NacosString &content) NACOS_THROW(NacosException);

    bool removeConfig(const NacosString &dataId, const NacosString &group) NACOS_THROW(NacosException);

    void addListener(const NacosString &dataId, const NacosString &group, Listener *listener) NACOS_THROW(NacosException);

    void removeListener(const NacosString &dataId, const NacosString &group, Listener *listener);

    HttpDelegate *getHttpDelegate() const { return _objectConfigData->_httpDelegate; };

    IHttpCli *getHttpCli() const { return _objectConfigData->_httpCli; };

    ServerListManager *getServerListManager() const { return _objectConfigData->_serverListManager; };

    ClientWorker *getClientWorker() const { return _objectConfigData->_clientWorker; };

    AppConfigManager *getAppConfigManager() const { return _objectConfigData->_appConfigManager; };

    void setHttpDelegate(HttpDelegate *httpDelegate) { _objectConfigData->_httpDelegate = httpDelegate; };

    void setHttpCli(IHttpCli *httpCli) { _objectConfigData->_httpCli = httpCli; };

    void setServerListManager(ServerListManager *svrListMgr) { _objectConfigData->_serverListManager = svrListMgr; };

    void setClientWorker(ClientWorker *clientWorker) { _objectConfigData->_clientWorker = clientWorker; };

    void setAppConfigManager(AppConfigManager *appConfigManager) { _objectConfigData->_appConfigManager = appConfigManager; };

    //ctors/dtor
    NacosConfigService(ObjectConfigData *objectConfigData) NACOS_THROW(NacosException);

    virtual ~NacosConfigService();
};
}//namespace nacos

#endif
