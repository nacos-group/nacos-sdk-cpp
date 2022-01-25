//
// Created by liuhanyu on 2020/8/30.
//

#include "src/factory/NacosServiceFactory.h"
#include "src/init/Init.h"
#include "src/naming/NacosNamingService.h"
#include "src/naming/NacosNamingMaintainService.h"
#include "ObjectConfigData.h"
#include "src/config/NacosConfigService.h"
#include "src/config/AppConfigManager.h"
#include "src/http/HttpDelegate.h"
#include "src/http/delegate/NoOpHttpDelegate.h"
#include "src/http/delegate/NacosAuthHttpDelegate.h"
#include "src/http/HTTPCli.h"
#include "src/naming/subscribe/EventDispatcher.h"
#include "src/naming/subscribe/SubscriptionPoller.h"
#include "src/naming/subscribe/UdpNamingServiceListener.h"
#include "src/naming/subscribe/HostReactor.h"
#include "src/security/SecurityManager.h"
#include "src/utils/ConfigParserUtils.h"
#include "src/utils/SequenceProvider.h"
#include "src/config/ConfigProxy.h"
#include "src/utils/DirUtils.h"

//Unlike Java, in cpp, there's no container, no spring to do the ORM job, so I have to handle it myself
namespace nacos{

volatile bool NacosServiceFactory::logSystemInitialized = false;

void buildSecurityManagerAndHttpDelegate(ObjectConfigData *objectConfigData) {
    AppConfigManager *appConfigManager = objectConfigData->_appConfigManager;
    if (appConfigManager->nacosAuthEnabled()) {
        //nacos authentication is enabled
        SecurityManager *securityManager = new SecurityManager(objectConfigData);
        objectConfigData->_securityManager = securityManager;
        HttpDelegate *httpDelegate = new NacosAuthHttpDelegate(objectConfigData);
        objectConfigData->_httpDelegate = httpDelegate;
    } else {
        HttpDelegate *httpDelegate = new NoOpHttpDelegate(objectConfigData);
        objectConfigData->_httpDelegate = httpDelegate;
    }
}

AppConfigManager *NacosServiceFactory::buildConfigManager(ObjectConfigData *objectConfigData) {
    //Create configuration data and load configs
    AppConfigManager *appConfigManager = NULL;

    if (configIsSet) {
        appConfigManager = new AppConfigManager(configFile);
        appConfigManager->loadConfig(configFile);
    } else {
        appConfigManager = new AppConfigManager(props);
    }
    objectConfigData->_appConfigManager = appConfigManager;

    return appConfigManager;
}

void NacosServiceFactory::initializeRuntimeLogSettings(AppConfigManager *_appConfigManager) {
    if (logSystemInitialized) {
        return;
    }

    {
        LockGuard __lockLogSystem(logSysInitLock);

        if (logSystemInitialized) {
            return;
        }

        logSystemInitialized = true;
        Properties copiedProps = _appConfigManager->getAllConfig();
        Logger::applyLogSettings(copiedProps);
    }
}

//FIXME:Memory leak at initializing stage, e.g.:
//when a HttpDelegate is allocated in CreateConfigService, after that an EXCEPTION is thrown during the initialization of ServerListManager
//the resource for HttpDelegate is never released
NamingService *NacosServiceFactory::CreateNamingService() NACOS_THROW(NacosException) {
    Init::doInit();
    checkConfig();
    ObjectConfigData *objectConfigData = new ObjectConfigData(NAMING);
    objectConfigData->name = "config";
    objectConfigData->encoding = "UTF-8";

    AppConfigManager *appConfigManager = buildConfigManager(objectConfigData);
    initializeRuntimeLogSettings(appConfigManager);

    //Create http client
    IHttpCli *httpCli= new HTTPCli();
    objectConfigData->_httpCli = httpCli;

    buildSecurityManagerAndHttpDelegate(objectConfigData);

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

    SubscriptionPoller *subscriptionPoller = new SubscriptionPoller(objectConfigData);
    objectConfigData->_subscriptionPoller = subscriptionPoller;

    UdpNamingServiceListener *udpNamingServiceListener = new UdpNamingServiceListener(objectConfigData);
    objectConfigData->_udpNamingServiceListener = udpNamingServiceListener;

    HostReactor *hostReactor = new HostReactor(objectConfigData);
    objectConfigData->_hostReactor = hostReactor;

    const NacosString &seqConfile = appConfigManager->get(PropertyKeyConst::INSTANCE_ID_SEQ_FILE);
    SequenceProvider<int64_t> *sequenceProvider = new SequenceProvider<int64_t>(seqConfile, 1, 10);
    objectConfigData->_sequenceProvider = sequenceProvider;

    objectConfigData->checkAssembledObject();
    NamingService *instance = new NacosNamingService(objectConfigData);

    log_debug("Created config data: %s", objectConfigData->name.c_str());
    return instance;
}

ConfigService *NacosServiceFactory::CreateConfigService() NACOS_THROW(NacosException) {
    Init::doInit();
    checkConfig();
    ObjectConfigData *objectConfigData = new ObjectConfigData(CONFIG);
    objectConfigData->name = "name";
    objectConfigData->encoding = "UTF-8";

    AppConfigManager *appConfigManager = buildConfigManager(objectConfigData);
    initializeRuntimeLogSettings(appConfigManager);

    //Create http client
    IHttpCli *httpCli = NULL;
    httpCli = new HTTPCli();
    objectConfigData->_httpCli = httpCli;

    buildSecurityManagerAndHttpDelegate(objectConfigData);

    //Create server manager
    ServerListManager *serverListManager = new ServerListManager(objectConfigData);
    objectConfigData->_serverListManager = serverListManager;

    ConfigProxy *configProxy = new ConfigProxy(objectConfigData);
    objectConfigData->_configProxy = configProxy;

    LocalSnapshotManager *localSnapshotManager = new LocalSnapshotManager(appConfigManager);
    objectConfigData->_localSnapshotManager = localSnapshotManager;
    ClientWorker *clientWorker = new ClientWorker(objectConfigData);
    objectConfigData->_clientWorker = clientWorker;
    objectConfigData->checkAssembledObject();

    ConfigService *instance = new NacosConfigService(objectConfigData);

    log_debug("Created config data: %s", objectConfigData->name.c_str());
    return instance;
}

NamingMaintainService *NacosServiceFactory::CreateNamingMaintainService() NACOS_THROW(NacosException){
    Init::doInit();
    checkConfig();
    ObjectConfigData *objectConfigData = new ObjectConfigData(MAINTAIN);
    objectConfigData->name = "config";
    objectConfigData->encoding = "UTF-8";

    AppConfigManager *appConfigManager = buildConfigManager(objectConfigData);
    initializeRuntimeLogSettings(appConfigManager);

    //Create http client
    IHttpCli *httpCli= new HTTPCli();
    objectConfigData->_httpCli = httpCli;

    buildSecurityManagerAndHttpDelegate(objectConfigData);

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

void NacosServiceFactory::checkConfig() NACOS_THROW(InvalidFactoryConfigException) {
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

    setConfig(DirUtils::getCwd() + "/" + ConfigConstant::DEFAULT_CONFIG_FILE);
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