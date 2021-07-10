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
#include "src/factory/ObjectConfigData.h"
#include "Compatibility.h"

namespace nacos{
class NamingProxy {
private:
    NacosString serverPort;
    ObjectConfigData *_objectConfigData;

    NacosString nacosDomain;

    NamingProxy();

    NacosString
    reqAPI(const NacosString &api, std::list <NacosString> &params, int method) NACOS_THROW(NacosException);

    NacosString
    callServer(const NacosString &api, std::list <NacosString> &params, const NacosString &curServer,
               int method) NACOS_THROW(NacosException);

    NacosString callServer(const NacosString &api, std::list <NacosString> &params,
                           const NacosString &curServer) NACOS_THROW(NacosException);

    std::list <NacosString> builderHeaders();

    long _hb_fail_wait;//Time to wait when a heartbeat request fails (in ms)

    static ListView<NacosString> nullResult;

    NacosString getDataToSign(const std::list <NacosString> &paramValues, NacosString &nowTimeMs);

public:
    NamingProxy(ObjectConfigData *objectConfigData);

    ~NamingProxy();

    //instance CRUD
    void registerService(const NacosString &serviceName, const NacosString &groupName,
                         Instance &instance) NACOS_THROW(NacosException);

    Instance getServiceInstance(const NacosString &serviceName,
                                const NacosString &ip, int port,
                                const std::map<NacosString, NacosString> &params) NACOS_THROW(NacosException);

    bool updateServiceInstance(const Instance &instance) NACOS_THROW(NacosException);

    void deregisterService(const NacosString &serviceName, Instance &instance) NACOS_THROW(NacosException);

    NacosString queryList(const NacosString &serviceName, const NacosString &groupName, const NacosString &clusters,
                        int udpPort, bool healthyOnly) NACOS_THROW(NacosException);

    //service CRUD
    ListView<NacosString> getServiceList(int page, int pageSize, const NacosString &groupName) NACOS_THROW(NacosException);

    ServiceInfo2 getServiceInfo(const NacosString &serviceName, const NacosString &groupName) NACOS_THROW(NacosException);

    bool deleteServiceInfo(const NacosString &serviceName, const NacosString &groupName) NACOS_THROW(NacosException);

    bool createServiceInfo(const ServiceInfo2 &serviceInfo2, naming::Selector *selector) NACOS_THROW(NacosException);

    bool updateServiceInfo(const ServiceInfo2 &serviceInfo2, naming::Selector *selector) NACOS_THROW(NacosException);

    inline NacosString getNamespaceId();

    long sendBeat(BeatInfo &beatInfo);

    bool serverHealthy();
};
}//namespace nacos

#endif
