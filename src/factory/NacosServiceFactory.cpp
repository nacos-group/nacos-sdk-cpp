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
    ObjectConfigData objectConfigData;
    objectConfigData.name = "config";
    NacosString encoding = "UTF-8";

    //Create configuration data and load configs
    AppConfigManager *appConfigManager = NULL;
    if (configIsSet) {
        appConfigManager = new AppConfigManager(configFile);
        appConfigManager->loadConfig(configFile);
    } else {
        appConfigManager = new AppConfigManager(props);
    }
    objectConfigData.appConfigManager = appConfigManager;

    //Create http client
    IHttpCli *httpCli= new HTTPCli();
    objectConfigData.httpCli = httpCli;

    HttpDelegate *httpDelegate = new NoOpHttpDelegate(httpCli, encoding);

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(httpDelegate, appConfigManager);
    objectConfigData.serverListManager = serverListManager;

    //Create naming service & heartbeat sender
    NamingProxy *namingProxy = new NamingProxy(httpDelegate, serverListManager, appConfigManager);
    objectConfigData.namingProxy = namingProxy;
    BeatReactor *beatReactor = new BeatReactor(namingProxy);
    objectConfigData.beatReactor = beatReactor;

    EventDispatcher *eventDispatcher = new EventDispatcher();

    TcpNamingServicePoller *tcpNamingServicePoller = new TcpNamingServicePoller(eventDispatcher, namingProxy, appConfigManager);

    NamingService *instance = new NacosNamingService(httpDelegate,
                                                     httpCli,
                                                     namingProxy,
                                                     beatReactor,
                                                     eventDispatcher,
                                                     tcpNamingServicePoller,
                                                     appConfigManager,
                                                     serverListManager);

    log_debug("Created config data: %s", objectConfigData.name.c_str());
    return instance;
}

ConfigService *NacosServiceFactory::CreateConfigService() throw(NacosException) {
    checkConfig();
    ObjectConfigData objectConfigData;
    objectConfigData.name = "name";

    //Create configuration data and load configs
    AppConfigManager *appConfigManager = NULL;
    if (configIsSet) {
        appConfigManager = new AppConfigManager(configFile);
        appConfigManager->loadConfig(configFile);
    } else {
        appConfigManager = new AppConfigManager(props);
    }
    objectConfigData.appConfigManager = appConfigManager;

    //Create http client
    IHttpCli *httpCli = NULL;
    httpCli = new HTTPCli();
    NacosString encoding = "UTF-8";
    HttpDelegate *httpDelegate = new NoOpHttpDelegate(httpCli, encoding);
    objectConfigData.httpCli = httpCli;

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(httpDelegate, appConfigManager);
    objectConfigData.serverListManager = serverListManager;

    ClientWorker *clientWorker = new ClientWorker(httpDelegate, appConfigManager, serverListManager);
    ConfigService *instance = new NacosConfigService(appConfigManager,
                                                     httpCli,
                                                     httpDelegate,
                                                     serverListManager,
                                                     clientWorker);

    log_debug("Created config data: %s", objectConfigData.name.c_str());
    return instance;
}

NamingMaintainService *NacosServiceFactory::CreateNamingMaintainService() throw(NacosException){
    checkConfig();
    ObjectConfigData objectConfigData;
    objectConfigData.name = "config";
    NacosString encoding = "UTF-8";

    //Create configuration data and load configs
    AppConfigManager *appConfigManager = NULL;
    if (configIsSet) {
        appConfigManager = new AppConfigManager(configFile);
        appConfigManager->loadConfig(configFile);
    } else {
        appConfigManager = new AppConfigManager(props);
    }
    objectConfigData.appConfigManager = appConfigManager;

    //Create http client
    IHttpCli *httpCli= new HTTPCli();
    objectConfigData.httpCli = httpCli;

    HttpDelegate *httpDelegate = new NoOpHttpDelegate(httpCli, encoding);

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(httpDelegate, appConfigManager);
    objectConfigData.serverListManager = serverListManager;

    //Create naming service & heartbeat sender
    NamingProxy *namingProxy = new NamingProxy(httpDelegate, serverListManager, appConfigManager);
    objectConfigData.namingProxy = namingProxy;

    NacosNamingMaintainService *instance = new NacosNamingMaintainService(namingProxy,
                                                                          httpDelegate,
                                                                          httpCli,
                                                                          appConfigManager,
                                                                          serverListManager);

    log_debug("Created config data: %s", objectConfigData.name.c_str());
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