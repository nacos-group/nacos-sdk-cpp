//
// Created by liuhanyu on 2020/8/30.
//

#include "factory/NacosServiceFactory.h"
#include "naming/NacosNamingService.h"
#include "config/ObjectConfigData.h"
#include "config/NacosConfigService.h"
#include "http/ServerHttpAgent.h"
#include "naming/subscribe/EventDispatcher.h"
#include "naming/subscribe/TcpNamingServicePoller.h"

NamingService *NacosServiceFactory::CreateNamingService() throw(NacosException) {
    checkConfig();
    ObjectConfigData objectConfigData;
    objectConfigData.name = "config";

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
    HTTPCli *httpcli = new HTTPCli();
    objectConfigData.httpCli = httpcli;

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(httpcli, appConfigManager);
    objectConfigData.serverListManager = serverListManager;

    //Create naming service & heartbeat sender
    NamingProxy *namingProxy = new NamingProxy(httpcli, serverListManager, appConfigManager);
    objectConfigData.namingProxy = namingProxy;
    BeatReactor *beatReactor = new BeatReactor(namingProxy);
    objectConfigData.beatReactor = beatReactor;

    EventDispatcher *eventDispatcher = new EventDispatcher();

    TcpNamingServicePoller *tcpNamingServicePoller = new TcpNamingServicePoller(eventDispatcher, namingProxy, appConfigManager);

    NamingService *instance = new NacosNamingService(httpcli, namingProxy, beatReactor, eventDispatcher, tcpNamingServicePoller, appConfigManager);

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
    HTTPCli *httpcli = new HTTPCli();
    objectConfigData.httpCli = httpcli;

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(httpcli, appConfigManager);
    objectConfigData.serverListManager = serverListManager;

    //Create naming service & heartbeat sender
    NamingProxy *namingProxy = new NamingProxy(httpcli, serverListManager, appConfigManager);
    objectConfigData.namingProxy = namingProxy;
    BeatReactor *beatReactor = new BeatReactor(namingProxy);
    objectConfigData.beatReactor = beatReactor;

    httpcli = new HTTPCli();
    NacosString encoding = "UTF-8";
    HttpAgent *httpAgent = new ServerHttpAgent(appConfigManager, httpcli, encoding, serverListManager);
    ClientWorker *clientWorker = new ClientWorker(httpAgent, appConfigManager);
    ConfigService *instance = new NacosConfigService(appConfigManager,
                                                     httpcli,
                                                     serverListManager,
                                                     httpAgent,
                                                     clientWorker);

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