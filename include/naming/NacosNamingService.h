#ifndef __NACOS_NAM_SVC_H_
#define __NACOS_NAM_SVC_H_

#include "naming/NamingService.h"
#include "naming/Instance.h"
#include "naming/NamingProxy.h"
#include "naming/subscribe/EventDispatcher.h"
#include "naming/subscribe/TcpNamingServicePoller.h"
#include "naming/beat/BeatReactor.h"
#include "config/ObjectConfigData.h"
#include "http/HTTPCli.h"
#include "NacosString.h"
#include "Properties.h"

class NacosNamingService : public NamingService {
private:
    HTTPCli *httpCli = NULL;
    NamingProxy *serverProxy = NULL;
    BeatReactor *beatReactor = NULL;
    EventDispatcher *_eventDispatcher = NULL;
    TcpNamingServicePoller *_tcpNamingServicePoller = NULL;
    AppConfigManager *_appConfigMgr = NULL;

    NacosNamingService();

    NacosString cacheDir;

    NacosString logName;
public:
    NacosNamingService(HTTPCli *httpCli, NamingProxy *serverProxy, BeatReactor *beatReactor, EventDispatcher *eventDispatcher, TcpNamingServicePoller *tcpNamingServicePoller, AppConfigManager *appConfigManager);

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
                                                 Selector<Instance> *predicate) throw(NacosException);

    std::list<Instance> getInstanceWithPredicate(const NacosString &serviceName,
                                                 const std::list <NacosString> &clusters,
                                                 Selector<Instance> *predicate) throw(NacosException);

    std::list<Instance> getInstanceWithPredicate(const NacosString &serviceName, const NacosString &groupName,
                                                 Selector<Instance> *predicate) throw(NacosException);

    std::list<Instance> getInstanceWithPredicate(const NacosString &serviceName,
                                                 Selector<Instance> *predicate) throw(NacosException);

    HTTPCli *getHttpCli() const { return httpCli; };

    NamingProxy *getServerProxy() const { return serverProxy; };

    BeatReactor *getBeatReactor() const { return beatReactor; };

    void setHttpCli(HTTPCli *_httpCli) { this->httpCli = _httpCli; };

    void setServerProxy(NamingProxy *_namingProxy) { this->serverProxy = _namingProxy; };

    void setBeatReactor(BeatReactor *_beatReactor) { this->beatReactor = beatReactor; };
};

#endif
