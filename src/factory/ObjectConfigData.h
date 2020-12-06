#ifndef __OBJ_CFG_DATA_H_
#define __OBJ_CFG_DATA_H_

#include "naming/NamingService.h"
#include "config/ConfigService.h"
#include "NacosExceptions.h"

namespace nacos{
class HttpDelegate;
class IHttpCli;
class NamingProxy;
class BeatReactor;
class EventDispatcher;
class TcpNamingServicePoller;
class AppConfigManager;
class ServerListManager;
class ClientWorker;
class LocalSnapshotManager;
class SecurityManager;

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
    void checkConfigService() throw(NacosException);
    void checkNamingService() throw(NacosException);
    void checkMaintainService() throw(NacosException);
public:
    ObjectConfigData(FactoryType theFactoryType);
    void checkAssembledObject() throw(NacosException);
    ~ObjectConfigData();
    NacosString name;
    HttpDelegate *_httpDelegate;
    IHttpCli *_httpCli;
    NamingProxy *_serverProxy;
    BeatReactor *_beatReactor;
    EventDispatcher *_eventDispatcher;
    TcpNamingServicePoller *_tcpNamingServicePoller;
    AppConfigManager *_appConfigManager;
    ServerListManager *_serverListManager;
    ClientWorker *_clientWorker;
    LocalSnapshotManager *_localSnapshotManager;
    SecurityManager *_securityManager;
};
}//namespace nacos

#endif