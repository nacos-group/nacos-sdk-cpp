//
// Created by liuhanyu on 2020/8/30.
//

#include "factory/NacosServiceFactory.h"
#include "src/init/Init.h"
#include "src/naming/NacosNamingService.h"
#include "src/naming/NacosNamingMaintainService.h"
#include "ObjectConfigData.h"
#include "src/config/NacosConfigService.h"
#include "src/http/HttpDelegate.h"
#include "src/http/delegate/NoOpHttpDelegate.h"
#include "src/http/HTTPCli.h"
#include "src/naming/subscribe/EventDispatcher.h"
#include "src/naming/subscribe/TcpNamingServicePoller.h"

//Unlike Java, in cpp, there's no container, no spring to do the ORM job, so I have to handle it myself
namespace nacos{

//FIXME:Memory leak at initializing stage, e.g.:
//when a HttpDelegate is allocated in CreateConfigService, after that an EXCEPTION is thrown during the initialization of ServerListManager
//the resource for HttpDelegate is never released
NamingService *NacosServiceFactory::CreateNamingService() throw(NacosException) {
    checkConfig();
    ObjectConfigData *objectConfigData = new ObjectConfigData(NAMING);
    objectConfigData->name = "config";
    NacosString encoding = "UTF-8";

    //Create configuration data and load configs
    AppConfigManager *appConfigManager = NULL;
    if (configIsSet) {
        appConfigManager = new AppConfigManager(configFile);
        appConfigManager->loadConfig(configFile);
    } else {
        appConfigManager = new AppConfigManager(props);
    }
    objectConfigData->_appConfigManager = appConfigManager;

    //Create http client
    IHttpCli *httpCli= new HTTPCli();
    objectConfigData->_httpCli = httpCli;

    HttpDelegate *httpDelegate = new NoOpHttpDelegate(objectConfigData);
    objectConfigData->_httpDelegate = httpDelegate;

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(objectConfigData);
    objectConfigData->_serverListManager = serverListManager;

    //Create naming service & heartbeat sender
    NamingProxy *namingProxy = new NamingProxy(objectConfigData);
    objectConfigData->_serverProxy = namingProxy;
    BeatReactor *beatReactor = new BeatReactor(objectConfigData);
    objectConfigData->_beatReactor = beatReactor;

    EventDispatcher *eventDispatcher = new EventDispatcher();
    objectConfigData->_eventDispatcher = eventDispatcher;

    TcpNamingServicePoller *tcpNamingServicePoller = new TcpNamingServicePoller(objectConfigData);
    objectConfigData->_tcpNamingServicePoller = tcpNamingServicePoller;

    objectConfigData->checkAssembledObject();
    NamingService *instance = new NacosNamingService(objectConfigData);

    log_debug("Created config data: %s", objectConfigData->name.c_str());
    return instance;
}

ConfigService *NacosServiceFactory::CreateConfigService() throw(NacosException) {
    checkConfig();
    ObjectConfigData *objectConfigData = new ObjectConfigData(CONFIG);
    objectConfigData->name = "name";

    //Create configuration data and load configs
    AppConfigManager *appConfigManager = NULL;
    if (configIsSet) {
        appConfigManager = new AppConfigManager(configFile);
        appConfigManager->loadConfig(configFile);
    } else {
        appConfigManager = new AppConfigManager(props);
    }
    objectConfigData->_appConfigManager = appConfigManager;

    //Create http client
    IHttpCli *httpCli = NULL;
    httpCli = new HTTPCli();
    NacosString encoding = "UTF-8";
    HttpDelegate *httpDelegate = new NoOpHttpDelegate(objectConfigData);
    objectConfigData->_httpDelegate = httpDelegate;
    objectConfigData->_httpCli = httpCli;

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(objectConfigData);
    objectConfigData->_serverListManager = serverListManager;

    LocalSnapshotManager *localSnapshotManager = new LocalSnapshotManager(appConfigManager);
    objectConfigData->_localSnapshotManager = localSnapshotManager;
    ClientWorker *clientWorker = new ClientWorker(objectConfigData);
    objectConfigData->_clientWorker = clientWorker;
    objectConfigData->checkAssembledObject();

    ConfigService *instance = new NacosConfigService(objectConfigData);

    log_debug("Created config data: %s", objectConfigData->name.c_str());
    return instance;
}

NamingMaintainService *NacosServiceFactory::CreateNamingMaintainService() throw(NacosException){
    checkConfig();
    ObjectConfigData *objectConfigData = new ObjectConfigData(MAINTAIN);
    objectConfigData->name = "config";
    NacosString encoding = "UTF-8";

    //Create configuration data and load configs
    AppConfigManager *appConfigManager = NULL;
    if (configIsSet) {
        appConfigManager = new AppConfigManager(configFile);
        appConfigManager->loadConfig(configFile);
    } else {
        appConfigManager = new AppConfigManager(props);
    }
    objectConfigData->_appConfigManager = appConfigManager;

    //Create http client
    IHttpCli *httpCli= new HTTPCli();
    objectConfigData->_httpCli = httpCli;

    HttpDelegate *httpDelegate = new NoOpHttpDelegate(objectConfigData);
    objectConfigData->_httpDelegate = httpDelegate;

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(objectConfigData);
    objectConfigData->_serverListManager = serverListManager;

    //Create naming service & heartbeat sender
    NamingProxy *namingProxy = new NamingProxy(objectConfigData);
    objectConfigData->_serverProxy = namingProxy;

    NacosNamingMaintainService *instance = new NacosNamingMaintainService(objectConfigData);

    log_debug("Created config data: %s", objectConfigData->name.c_str());
    return instance;
}

NacosServiceFactory::~NacosServiceFactory() {

}


void NacosServiceFactory::checkConfig() throw(InvalidFactoryConfigException) {
    if (!configIsSet && !propsIsSet) {
        throw InvalidFactoryConfigException();
    }
}

void NacosServiceFactory::setConfig(const NacosString &_configFile) {
    configIsSet = true;
    configFile = _configFile;
};

void NacosServiceFactory::setProps(Properties &_props) {
    propsIsSet = true;
    props = _props;
};

NacosServiceFactory::NacosServiceFactory() {
    configIsSet = false;
    propsIsSet = false;
}

NacosServiceFactory::NacosServiceFactory(const NacosString &_configFile) {
    configIsSet = false;
    propsIsSet = false;
    setConfig(_configFile);
}

NacosServiceFactory::NacosServiceFactory(Properties &_props) {
    configIsSet = false;
    propsIsSet = false;
    setProps(_props);
}
}//namespace nacos