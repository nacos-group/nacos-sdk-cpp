#include "src/naming/NacosNamingService.h"
#include "src/naming/beat/BeatReactor.h"
#include "utils/NamingUtils.h"
#include "utils/UtilAndComs.h"
#include "utils/ParamUtils.h"
#include "PropertyKeyConst.h"
#include "src/json/JSON.h"

using namespace std;
using nacos::naming::selectors::Selector;

namespace nacos{
NacosNamingService::NacosNamingService(HttpDelegate *httpDelegate, IHttpCli *httpCli, NamingProxy *_serverProxy, BeatReactor *_beatReactor,
                                       EventDispatcher *eventDispatcher, TcpNamingServicePoller *tcpNamingServicePoller,
                                       AppConfigManager *appConfigManager, ServerListManager *serverListManager) {
    _appConfigMgr = appConfigManager;
    _httpCli = httpCli;
    _httpDelegate = httpDelegate;
    serverProxy = _serverProxy;
    beatReactor = _beatReactor;
    _eventDispatcher = eventDispatcher;
    _tcpNamingServicePoller = tcpNamingServicePoller;
    _serverListManager = serverListManager;
    beatReactor->start();
    _tcpNamingServicePoller->start();
}

NacosNamingService::~NacosNamingService() {
    if (beatReactor != NULL) {
        delete beatReactor;
    }
    beatReactor = NULL;

    if (_tcpNamingServicePoller != NULL)
    {
        delete _tcpNamingServicePoller;
    }
    _tcpNamingServicePoller = NULL;

    if (_eventDispatcher != NULL)
    {
        delete _eventDispatcher;
    }
    _eventDispatcher = NULL;

    if (serverProxy != NULL) {
        delete serverProxy;
    }
    serverProxy = NULL;

    if (_serverListManager != NULL) {
        delete _serverListManager;
    }
    _serverListManager = NULL;

    if (_httpDelegate != NULL) {
        delete _httpDelegate;
    }
    _httpDelegate = NULL;

    if (_httpCli != NULL) {
        delete _httpCli;
    }
    _httpCli = NULL;

    if (_appConfigMgr != NULL)
    {
        delete _appConfigMgr;
    }
    _appConfigMgr = NULL;
}

void NacosNamingService::registerInstance
        (
                const NacosString &serviceName,
                const NacosString &ip,
                int port
        ) throw(NacosException) {
    registerInstance(serviceName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::registerInstance
        (
                const NacosString &serviceName,
                const NacosString &groupName,
                const NacosString &ip,
                int port
        ) throw(NacosException) {
    registerInstance(serviceName, groupName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::registerInstance
        (
                const NacosString &serviceName,
                const NacosString &ip,
                int port,
                const NacosString &clusterName
        ) throw(NacosException) {
    registerInstance(serviceName, Constants::DEFAULT_GROUP, ip, port, clusterName);
}

void NacosNamingService::registerInstance
        (
                const NacosString &serviceName,
                const NacosString &groupName,
                const NacosString &ip,
                int port,
                const NacosString &clusterName
        ) throw(NacosException) {
    Instance instance;
    instance.ip = ip;
    instance.port = port;
    instance.weight = 1.0D;
    instance.clusterName = clusterName;

    registerInstance(serviceName, groupName, instance);
}

void NacosNamingService::registerInstance
        (
                const NacosString &serviceName,
                Instance &instance
        ) throw(NacosException) {
    registerInstance(serviceName, Constants::DEFAULT_GROUP, instance);
}

void NacosNamingService::registerInstance
        (
                const NacosString &serviceName,
                const NacosString &groupName,
                Instance &instance
        ) throw(NacosException) {

    if (instance.ephemeral) {
        BeatInfo beatInfo;
        beatInfo.serviceName = NamingUtils::getGroupedName(serviceName, groupName);
        beatInfo.ip = instance.ip;
        beatInfo.port = instance.port;
        beatInfo.cluster = instance.clusterName;
        beatInfo.weight = instance.weight;
        beatInfo.metadata = instance.metadata;
        beatInfo.scheduled = false;

        beatReactor->addBeatInfo(NamingUtils::getGroupedName(serviceName, groupName), beatInfo);
    }

    serverProxy->registerService(NamingUtils::getGroupedName(serviceName, groupName), groupName, instance);
}

void NacosNamingService::deregisterInstance
        (
                const NacosString &serviceName,
                const NacosString &ip,
                int port
        ) throw(NacosException) {
    deregisterInstance(serviceName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::deregisterInstance
        (
                const NacosString &serviceName,
                const NacosString &groupName,
                const NacosString &ip,
                int port
        ) throw(NacosException) {
    deregisterInstance(serviceName, groupName, ip, port, Constants::DEFAULT_CLUSTER_NAME);
}

void NacosNamingService::deregisterInstance
        (
                const NacosString &serviceName,
                const NacosString &ip,
                int port,
                const NacosString &clusterName
        ) throw(NacosException) {
    deregisterInstance(serviceName, Constants::DEFAULT_GROUP, ip, port, clusterName);
}

void NacosNamingService::deregisterInstance
        (
                const NacosString &serviceName,
                const NacosString &groupName,
                const NacosString &ip,
                int port,
                const NacosString &clusterName
        ) throw(NacosException) {
    Instance instance;
    instance.ip = ip;
    instance.port = port;
    instance.clusterName = clusterName;

    deregisterInstance(serviceName, groupName, instance);
}

void NacosNamingService::deregisterInstance
        (
                const NacosString &serviceName,
                const NacosString &groupName,
                Instance &instance
        ) throw(NacosException) {
    beatReactor->removeBeatInfo(NamingUtils::getGroupedName(serviceName, groupName), instance.ip, instance.port);
    serverProxy->deregisterService(NamingUtils::getGroupedName(serviceName, groupName), instance);
}

list <Instance> NacosNamingService::getAllInstances
        (
                const NacosString &serviceName
        ) throw(NacosException) {
    list <NacosString> clusters;
    return getAllInstances(serviceName, clusters);
}

list <Instance> NacosNamingService::getAllInstances
        (
                const NacosString &serviceName,
                const NacosString &groupName
        ) throw(NacosException) {
    list <NacosString> clusters;
    return getAllInstances(serviceName, groupName, clusters);
}

list <Instance> NacosNamingService::getAllInstances
        (
                const NacosString &serviceName,
                const list <NacosString> &clusters
        ) throw(NacosException) {
    return getAllInstances(serviceName, Constants::DEFAULT_GROUP, clusters);
}

list <Instance> NacosNamingService::getAllInstances
        (
                const NacosString &serviceName,
                const NacosString &groupName,
                const list <NacosString> &clusters
        ) throw(NacosException) {
    ServiceInfo serviceInfo;
    //TODO:cache and failover
    NacosString clusterString = ParamUtils::Implode(clusters);
    NacosString result = serverProxy->queryList(serviceName, clusterString, 0/*What should be filled in UDPPort??*/,
                                                false);
    serviceInfo = JSON::JsonStr2ServiceInfo(result);
    list <Instance> hostlist = serviceInfo.getHosts();
    return hostlist;
}

void NacosNamingService::subscribe(const NacosString &serviceName, EventListener *listener) throw (NacosException)
{
    list<NacosString> clusters;//empty cluster
    subscribe(serviceName, Constants::DEFAULT_GROUP, clusters, listener);
}


void NacosNamingService::subscribe
(
    const NacosString &serviceName,
    const NacosString &groupName,
    EventListener *listener
) throw (NacosException)
{
    list<NacosString> clusters;//empty cluster
    subscribe(serviceName, groupName, clusters, listener);
}

void NacosNamingService::subscribe(
    const NacosString &serviceName,
    const std::list<NacosString> &clusters,
    EventListener *listener
) throw (NacosException)
{
    subscribe(serviceName, Constants::DEFAULT_GROUP, clusters, listener);
}

void NacosNamingService::subscribe
(
        const NacosString &serviceName,
        const NacosString &groupName,
        const std::list<NacosString> &clusters,
        EventListener *listener
) throw (NacosException)
{
    NacosString clusterName = ParamUtils::Implode(clusters);
    NacosString groupedName = NamingUtils::getGroupedName(serviceName, groupName);
    if (!_eventDispatcher->addListener(groupedName, clusterName, listener)){
        return;//The listener is already listening to the service specified, no need to add to the polling list
    }
    _tcpNamingServicePoller->addPollItem(serviceName, groupName, clusterName);
}


void NacosNamingService::unsubscribe(
    const NacosString &serviceName,
    const NacosString &groupName,
    const std::list<NacosString> &clusters,
    EventListener *listener
) throw (NacosException)
{
    NacosString clusterName = ParamUtils::Implode(clusters);
    NacosString groupedName = NamingUtils::getGroupedName(serviceName, groupName);
    int remainingListener;
    if (!_eventDispatcher->removeListener(groupedName, clusterName, listener, remainingListener)) {
        return;//The listener is not in the list or it is already removed
    }
    if (remainingListener == 0) {
        //Since there's no more listeners listening to this service, remove it from the polling list
        _tcpNamingServicePoller->removePollItem(serviceName, groupName, clusterName);
    }
}

void NacosNamingService::unsubscribe
(
    const NacosString &serviceName,
    const std::list<NacosString> &clusters,
    EventListener *listener
) throw (NacosException)
{
    unsubscribe(serviceName, Constants::DEFAULT_GROUP, clusters, listener);
}

void NacosNamingService::unsubscribe
(
    const NacosString &serviceName,
    const NacosString &groupName,
    EventListener *listener
) throw (NacosException)
{
    list<NacosString> clusters;
    unsubscribe(serviceName, groupName, clusters, listener);
}

void NacosNamingService::unsubscribe(const NacosString &serviceName, EventListener *listener) throw (NacosException)
{
    list<NacosString> clusters;
    unsubscribe(serviceName, Constants::DEFAULT_GROUP, clusters, listener);
}

ListView<NacosString> NacosNamingService::getServiceList(int pageNo, int pageSize) throw (NacosException) {
    return serverProxy->getServiceList(pageNo, pageSize, Constants::DEFAULT_GROUP);
}

ListView<NacosString> NacosNamingService::getServiceList(int pageNo, int pageSize, const NacosString &groupName) throw (NacosException){
    return serverProxy->getServiceList(pageNo, pageSize, groupName);
}

list<Instance> NacosNamingService::getInstanceWithPredicate
(
    const NacosString &serviceName,
    const NacosString &groupName,
    const std::list <NacosString> &clusters,
    Selector<Instance> *predicate
) throw(NacosException)
{
    list<Instance> allInstances = getAllInstances(serviceName, groupName, clusters);
    if (predicate) {
        return predicate->select(allInstances);
    } else {
        return allInstances;
    }
}

list<Instance> NacosNamingService::getInstanceWithPredicate
(
    const NacosString &serviceName,
    const std::list <NacosString> &clusters,
    Selector<Instance> *predicate
) throw(NacosException)
{
    list<Instance> allInstances = getAllInstances(serviceName, Constants::DEFAULT_GROUP, clusters);
    if (predicate) {
        return predicate->select(allInstances);
    } else {
        return allInstances;
    }
}

list<Instance> NacosNamingService::getInstanceWithPredicate
(
    const NacosString &serviceName,
    const NacosString &groupName,
    Selector<Instance> *predicate
) throw(NacosException)
{
    list<NacosString> clusters;
    list<Instance> allInstances = getAllInstances(serviceName, groupName, clusters);
    if (predicate) {
        return predicate->select(allInstances);
    } else {
        return allInstances;
    }
}

list<Instance> NacosNamingService::getInstanceWithPredicate
(
    const NacosString &serviceName,
    Selector<Instance> *predicate
) throw(NacosException)
{
    list<NacosString> clusters;
    list<Instance> allInstances = getAllInstances(serviceName, Constants::DEFAULT_GROUP, clusters);
    if (predicate) {
        return predicate->select(allInstances);
    } else {
        return allInstances;
    }
}
}//namespace nacos
