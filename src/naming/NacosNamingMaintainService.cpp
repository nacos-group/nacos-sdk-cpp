#include "src/naming/NacosNamingMaintainService.h"

using namespace std;

namespace nacos{

NacosNamingMaintainService::NacosNamingMaintainService
(
    NamingProxy *namingProxy,
    HttpDelegate *httpDelegate,
    IHttpCli *httpCli,
    AppConfigManager *appConfigManager,
    ServerListManager *serverListManager
) {
    _namingProxy = namingProxy;
    _httpDelegate = httpDelegate;
    _httpCli = httpCli;
    _appConfigManager = appConfigManager;
    _serverListManager = serverListManager;
}

bool NacosNamingMaintainService::updateInstance
(
    const NacosString &serviceName,
    const NacosString & groupName,
    const Instance &instance
) throw(NacosException) {
    Instance paramInstance = instance;
    paramInstance.serviceName = serviceName;
    paramInstance.groupName = groupName;
    return _namingProxy->updateServiceInstance(paramInstance);
}

ServiceInfo2 NacosNamingMaintainService::queryService
(
    const NacosString &serviceName,
    const NacosString &groupName
) throw(NacosException) {
    return _namingProxy->getServiceInfo(serviceName, groupName);
}

bool NacosNamingMaintainService::createService(const ServiceInfo2 &service, naming::Selector *selector) throw(NacosException) {
    ServiceInfo2 parmServiceInfo = service;
    if (!parmServiceInfo.isGroupNameSet()) {
        parmServiceInfo.setGroupName(Constants::DEFAULT_GROUP);
    }
    return _namingProxy->createServiceInfo(parmServiceInfo, selector);
}

bool NacosNamingMaintainService::deleteService(const NacosString &serviceName, const NacosString &groupName) throw(NacosException) {
    return _namingProxy->deleteServiceInfo(serviceName, groupName);
}

bool NacosNamingMaintainService::updateService(const ServiceInfo2 &service, naming::Selector *selector) throw(NacosException) {
    return _namingProxy->updateServiceInfo(service, selector);
}

NacosNamingMaintainService::~NacosNamingMaintainService() {
    if (_namingProxy != NULL) {
        delete _namingProxy;
        _namingProxy = NULL;
    }
    if (_serverListManager != NULL) {
        _serverListManager->stop();
        delete _serverListManager;
        _serverListManager = NULL;
    }
    if (_appConfigManager != NULL) {
        delete _appConfigManager;
        _appConfigManager = NULL;
    }
    if (_httpDelegate != NULL) {
        delete _httpDelegate;
        _httpDelegate = NULL;
    }
    if (_httpCli != NULL) {
        delete _httpCli;
        _httpCli = NULL;
    }
}

}//namespace nacos
