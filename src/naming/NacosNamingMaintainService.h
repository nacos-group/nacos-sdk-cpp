#ifndef __NACOS_NAM_MTN_SVC_H_
#define __NACOS_NAM_MTN_SVC_H_

#include "naming/NamingMaintainService.h"
#include "naming/Instance.h"
#include "src/naming/NamingProxy.h"
#include "src/http/IHttpCli.h"
#include "NacosString.h"
#include "Properties.h"

namespace nacos{

/**
 * NacosNamingMaintainService
 * Maintain functions for Nacos
 *
 * @author liaochuntao
 * @author Liu, Hanyu
 * Maintain service
 * Special thanks to @liaochuntao
 */
class NacosNamingMaintainService : public NamingMaintainService {
private:
    NacosNamingMaintainService();
    NamingProxy *_namingProxy;
    HttpDelegate *_httpDelegate;
    IHttpCli *_httpCli;
    AppConfigManager *_appConfigManager;
    ServerListManager *_serverListManager;
public:
    NacosNamingMaintainService(
            NamingProxy *namingProxy,
            HttpDelegate *httpDelegate,
            IHttpCli *httpCli,
            AppConfigManager *appConfigManager,
            ServerListManager *serverListManager
    );
    bool updateInstance(const NacosString &serviceName, const NacosString & groupName, const Instance &instance) throw(NacosException);
    ServiceInfo2 queryService(const NacosString &serviceName, const NacosString &groupName) throw(NacosException);
    bool createService(const ServiceInfo2 &service, naming::Selector *selector) throw(NacosException);
    bool deleteService(const NacosString &serviceName, const NacosString &groupName) throw(NacosException);
    bool updateService(const ServiceInfo2 &service, naming::Selector *selector) throw(NacosException);
    virtual ~NacosNamingMaintainService();
};
}//namespace nacos

#endif
