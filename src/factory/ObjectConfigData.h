#ifndef __OBJ_CFG_DATA_H_
#define __OBJ_CFG_DATA_H_

#include "naming/NamingService.h"
#include "config/ConfigService.h"
#include "NacosExceptions.h"
#include "Compatibility.h"
#include <stdint.h>

namespace nacos{
class HttpDelegate;
class IHttpCli;
class NamingProxy;
class BeatReactor;
class EventDispatcher;
class SubscriptionPoller;
class AppConfigManager;
class ServerListManager;
class ClientWorker;
class LocalSnapshotManager;
class SecurityManager;
class UdpNamingServiceListener;
class HostReactor;
class ConfigProxy;
template <typename T>class SequenceProvider;

enum FactoryType {
    CONFIG = 0,
    NAMING = 1,
    MAINTAIN = 2
};

class ObjectConfigData {
private:
    FactoryType factoryType;
    void destroyConfigService();
    void destroyNamingService();
    void destroyMaintainService();
    //These functions are designed to prevent coding problems
    //(i.e.: forget to initialize HttpDelegate for a ConfigService) rather than run-time errors
    void checkConfigService() NACOS_THROW(NacosException);
    void checkNamingService() NACOS_THROW(NacosException);
    void checkMaintainService() NACOS_THROW(NacosException);
    NacosString objectId;
public:
    const NacosString &getObjectId() const { return objectId; };
    ObjectConfigData(FactoryType theFactoryType);
    void checkAssembledObject() NACOS_THROW(NacosException);
    ~ObjectConfigData();
    NacosString name;
    NacosString encoding;
    HttpDelegate *_httpDelegate;
    IHttpCli *_httpCli;
    NamingProxy *_serverProxy;
    BeatReactor *_beatReactor;
    EventDispatcher *_eventDispatcher;
    SubscriptionPoller *_subscriptionPoller;
    AppConfigManager *_appConfigManager;
    ServerListManager *_serverListManager;
    ClientWorker *_clientWorker;
    LocalSnapshotManager *_localSnapshotManager;
    SecurityManager *_securityManager;
    UdpNamingServiceListener *_udpNamingServiceListener;
    HostReactor *_hostReactor;
    SequenceProvider<int64_t> *_sequenceProvider;
    ConfigProxy *_configProxy;
};
}//namespace nacos

#endif