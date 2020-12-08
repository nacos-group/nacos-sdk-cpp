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
NacosNamingService::NacosNamingService(ObjectConfigData *objectConfigData) {
    _objectConfigData = objectConfigData;
    _objectConfigData->_beatReactor->start();
    _objectConfigData->_tcpNamingServicePoller->start();

    if (_objectConfigData->_appConfigManager->nacosAuthEnabled()) {
        _objectConfigData->_securityManager->login();
        _objectConfigData->_securityManager->start();
    }
}

NacosNamingService::~NacosNamingService() {
    delete _objectConfigData;
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

        _objectConfigData->_beatReactor->addBeatInfo(NamingUtils::getGroupedName(serviceName, groupName), beatInfo);
    }

    _objectConfigData->_serverProxy->registerService(NamingUtils::getGroupedName(serviceName, groupName), groupName, instance);
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
    _objectConfigData->_beatReactor->removeBeatInfo(NamingUtils::getGroupedName(serviceName, groupName), instance.ip, instance.port);
    _objectConfigData->_serverProxy->deregisterService(NamingUtils::getGroupedName(serviceName, groupName), instance);
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
    NacosString result = _objectConfigData->_serverProxy->queryList(serviceName, clusterString, 0/*What should be filled in UDPPort??*/,
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
    if (!_objectConfigData->_eventDispatcher->addListener(groupedName, clusterName, listener)){
        return;//The listener is already listening to the service specified, no need to add to the polling list
    }
    _objectConfigData->_tcpNamingServicePoller->addPollItem(serviceName, groupName, clusterName);
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
    if (!_objectConfigData->_eventDispatcher->removeListener(groupedName, clusterName, listener, remainingListener)) {
        return;//The listener is not in the list or it is already removed
    }
    if (remainingListener == 0) {
        //Since there's no more listeners listening to this service, remove it from the polling list
        _objectConfigData->_tcpNamingServicePoller->removePollItem(serviceName, groupName, clusterName);
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
    return _objectConfigData->_serverProxy->getServiceList(pageNo, pageSize, Constants::DEFAULT_GROUP);
}

ListView<NacosString> NacosNamingService::getServiceList(int pageNo, int pageSize, const NacosString &groupName) throw (NacosException){
    return _objectConfigData->_serverProxy->getServiceList(pageNo, pageSize, groupName);
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
