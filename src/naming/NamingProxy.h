#ifndef __NAMING_PROXY_H_
#define __NAMING_PROXY_H_

#include <map>
#include <list>
#include "NacosString.h"
#include "NacosExceptions.h"
#include "src/http/HttpDelegate.h"
#include "naming/Instance.h"
#include "src/naming/beat/BeatInfo.h"
#include "src/server/ServerListManager.h"
#include "naming/ListView.h"
#include "naming/ServiceInfo2.h"

namespace nacos{
class NamingProxy {
private:
    NacosString serverPort;
    HttpDelegate *_httpDelegate = NULL;
    ServerListManager *serverListManager;
    AppConfigManager *appConfigManager;

    NacosString nacosDomain;

    NamingProxy();

    NacosString
    reqAPI(const NacosString &api, std::list <NacosString> &params, int method) throw(NacosException);

    NacosString
    callServer(const NacosString &api, std::list <NacosString> &params, const NacosString &curServer,
               int method) throw(NacosException);

    NacosString callServer(const NacosString &api, std::list <NacosString> &params,
                           const NacosString &curServer) throw(NacosException);

    std::list <NacosString> builderHeaders();

    long _http_req_timeout;
    long _hb_fail_wait;//Time to wait when a heartbeat request fails (in ms)

    static ListView<NacosString> nullResult;
public:
    NamingProxy(HttpDelegate *httpDelegate, ServerListManager *serverListManager, AppConfigManager *appConfigManager);

    ~NamingProxy();

    //instance CRUD
    void registerService(const NacosString &serviceName, const NacosString &groupName,
                         Instance &instance) throw(NacosException);

    Instance getServiceInstance(const NacosString &serviceName,
                                const NacosString &ip, int port,
                                const std::map<NacosString, NacosString> &params) throw(NacosException);

    bool updateServiceInstance(const Instance &instance) throw(NacosException);

    void deregisterService(const NacosString &serviceName, Instance &instance) throw(NacosException);

    NacosString queryList(const NacosString &serviceName, const NacosString &clusters, int udpPort,
                          bool healthyOnly) throw(NacosException);

    //service CRUD
    ListView<NacosString> getServiceList(int page, int pageSize, const NacosString &groupName) throw(NacosException);

    ServiceInfo2 getServiceInfo(const NacosString &serviceName, const NacosString &groupName) throw(NacosException);

    bool deleteServiceInfo(const NacosString &serviceName, const NacosString &groupName) throw(NacosException);

    bool createServiceInfo(const ServiceInfo2 &serviceInfo2, naming::Selector *selector) throw(NacosException);

    bool updateServiceInfo(const ServiceInfo2 &serviceInfo2, naming::Selector *selector) throw(NacosException);

    inline NacosString getNamespaceId();

    long sendBeat(BeatInfo &beatInfo);

    bool serverHealthy();
};
}//namespace nacos

#endif
