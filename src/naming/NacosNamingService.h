#ifndef __NACOS_NAM_SVC_H_
#define __NACOS_NAM_SVC_H_

#include "naming/NamingService.h"
#include "naming/Instance.h"
#include "src/naming/NamingProxy.h"
#include "src/naming/subscribe/EventDispatcher.h"
#include "src/naming/beat/BeatReactor.h"
#include "src/http/IHttpCli.h"
#include "NacosString.h"
#include "Properties.h"
#include "src/factory/ObjectConfigData.h"

namespace nacos{
class NacosNamingService : public NamingService {
private:
    ObjectConfigData *_objectConfigData;

    NacosNamingService();

    NacosString cacheDir;

    NacosString logName;
public:
    NacosNamingService(ObjectConfigData *objectConfigData);

    ~NacosNamingService();

    void registerInstance(const NacosString &serviceName, const NacosString &ip, int port) throw(NacosException);

    void registerInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip,
                          int port) throw(NacosException);

    void registerInstance(const NacosString &serviceName, const NacosString &ip, int port,
                          const NacosString &clusterName) throw(NacosException);

    void registerInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip, int port,
                          const NacosString &clusterName) throw(NacosException);

    void registerInstance(const NacosString &serviceName, Instance &instance) throw(NacosException);

    void registerInstance(const NacosString &serviceName, const NacosString &groupName,
                          Instance &instance) throw(NacosException);

    void deregisterInstance(const NacosString &serviceName, const NacosString &ip, int port) throw(NacosException);

    void deregisterInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip,
                            int port) throw(NacosException);

    void deregisterInstance(const NacosString &serviceName, const NacosString &ip, int port,
                            const NacosString &clusterName) throw(NacosException);

    void
    deregisterInstance(const NacosString &serviceName, const NacosString &groupName, const NacosString &ip, int port,
                       const NacosString &clusterName) throw(NacosException);

    void deregisterInstance(const NacosString &serviceName, const NacosString &groupName,
                            Instance &instance) throw(NacosException);

    std::list <Instance> getAllInstances(const NacosString &serviceName) throw(NacosException);

    std::list <Instance>
    getAllInstances(const NacosString &serviceName, const NacosString &groupName) throw(NacosException);

    std::list <Instance>
    getAllInstances(const NacosString &serviceName, const std::list <NacosString> &clusters) throw(NacosException);

    std::list <Instance> getAllInstances(const NacosString &serviceName, const NacosString &groupName,
                                         const std::list <NacosString> &clusters) throw(NacosException);

    void subscribe(const NacosString &serviceName, EventListener *listener) throw (NacosException);

    void subscribe(const NacosString &serviceName, const NacosString &groupName, const std::list<NacosString> &clusters, EventListener *listener) throw (NacosException);

    void subscribe(const NacosString &serviceName, const NacosString &groupName, EventListener *listener) throw (NacosException);

    void subscribe(const NacosString &serviceName, const std::list<NacosString> &clusters, EventListener *listener) throw (NacosException);

    void unsubscribe(const NacosString &serviceName, EventListener *listener) throw (NacosException);

    void unsubscribe(const NacosString &serviceName, const NacosString &groupName, EventListener *listener) throw (NacosException);

    void unsubscribe(const NacosString &serviceName, const std::list<NacosString> &clusters, EventListener *listener) throw (NacosException);

    void unsubscribe(const NacosString &serviceName, const NacosString &groupName, const std::list<NacosString> &clusters, EventListener *listener) throw (NacosException);

    ListView<NacosString> getServiceList(int pageNo, int pageSize) throw (NacosException);

    ListView<NacosString> getServiceList(int pageNo, int pageSize, const NacosString &groupName) throw (NacosException);

    std::list<Instance> getInstanceWithPredicate(const NacosString &serviceName, const NacosString &groupName,
                                                 const std::list <NacosString> &clusters,
                                                 nacos::naming::selectors::Selector<Instance> *predicate) throw(NacosException);

    std::list<Instance> getInstanceWithPredicate(const NacosString &serviceName,
                                                 const std::list <NacosString> &clusters,
                                                 nacos::naming::selectors::Selector<Instance> *predicate) throw(NacosException);

    std::list<Instance> getInstanceWithPredicate(const NacosString &serviceName, const NacosString &groupName,
                                                 nacos::naming::selectors::Selector<Instance> *predicate) throw(NacosException);

    std::list<Instance> getInstanceWithPredicate(const NacosString &serviceName,
                                                 nacos::naming::selectors::Selector<Instance> *predicate) throw(NacosException);

    IHttpCli *getHttpCli() const { return _objectConfigData->_httpCli; };

    NamingProxy *getServerProxy() const { return _objectConfigData->_serverProxy; };

    BeatReactor *getBeatReactor() const { return _objectConfigData->_beatReactor; };

    void setHttpCli(IHttpCli *httpCli) { this->_objectConfigData->_httpCli = httpCli; };

    void setServerProxy(NamingProxy *_namingProxy) { this->_objectConfigData->_serverProxy = _namingProxy; };

    void setBeatReactor(BeatReactor *_beatReactor) { this->_objectConfigData->_beatReactor = _beatReactor; };
};
}//namespace nacos

#endif
