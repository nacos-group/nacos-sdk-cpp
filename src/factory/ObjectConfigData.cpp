#include "ObjectConfigData.h"
#include "src/http/HttpDelegate.h"
#include "src/naming/NamingProxy.h"
#include "src/naming/beat/BeatReactor.h"
#include "src/naming/subscribe/EventDispatcher.h"
#include "src/naming/subscribe/SubscriptionPoller.h"
#include "src/naming/subscribe/UdpNamingServiceListener.h"
#include "src/naming/subscribe/HostReactor.h"
#include "src/listen/ClientWorker.h"
#include "src/security/SecurityManager.h"
#include "src/utils/UuidUtils.h"
#include "src/utils/SequenceProvider.h"
#include "src/config/ConfigProxy.h"
#include "src/debug/DebugAssertion.h"

namespace nacos{

ObjectConfigData::ObjectConfigData(FactoryType theFactoryType) {
    objectId = UuidUtils::generateUuid();
    factoryType = theFactoryType;
    _httpDelegate = NULL;
    _httpCli = NULL;
    _serverProxy = NULL;
    _beatReactor = NULL;
    _eventDispatcher = NULL;
    _subscriptionPoller = NULL;
    _appConfigManager = NULL;
    _serverListManager = NULL;
    _clientWorker = NULL;
    _localSnapshotManager = NULL;
    _securityManager = NULL;
    _configProxy = NULL;
}

void ObjectConfigData::checkNamingService() NACOS_THROW(NacosException) {
    if (factoryType != NAMING) {
        throw NacosException(NacosException::INVALID_PARAM, "Invalid configuration for naming service, please check");
    }

    NACOS_ASSERT(_httpDelegate != NULL);
    NACOS_ASSERT(_httpCli != NULL);
    NACOS_ASSERT(_serverProxy != NULL);
    NACOS_ASSERT(_beatReactor != NULL);
    NACOS_ASSERT(_eventDispatcher != NULL);
    NACOS_ASSERT(_subscriptionPoller != NULL);
    NACOS_ASSERT(_hostReactor != NULL);
    NACOS_ASSERT(_appConfigManager != NULL);
    NACOS_ASSERT(_serverListManager != NULL);
    NACOS_ASSERT(_udpNamingServiceListener != NULL);
    NACOS_ASSERT(_udpNamingServiceListener != NULL);
    NACOS_ASSERT(_sequenceProvider != NULL);
}

void ObjectConfigData::checkConfigService() NACOS_THROW(NacosException) {
    if (factoryType != CONFIG) {
        throw NacosException(NacosException::INVALID_PARAM, "Invalid configuration for config service, please check");
    }

    NACOS_ASSERT(_appConfigManager != NULL);
    NACOS_ASSERT(_httpCli != NULL);
    NACOS_ASSERT(_httpDelegate != NULL);
    NACOS_ASSERT(_serverListManager != NULL);
    NACOS_ASSERT(_clientWorker != NULL);
    NACOS_ASSERT(_localSnapshotManager != NULL);
    NACOS_ASSERT(_configProxy != NULL);
}

void ObjectConfigData::checkMaintainService() NACOS_THROW(NacosException) {
    if (factoryType != MAINTAIN) {
        throw NacosException(NacosException::INVALID_PARAM, "Invalid configuration for maintain service, please check");
    }

    NACOS_ASSERT(_serverProxy != NULL);
    NACOS_ASSERT(_httpDelegate != NULL);
    NACOS_ASSERT(_httpCli != NULL);
    NACOS_ASSERT(_appConfigManager != NULL);
    NACOS_ASSERT(_serverListManager != NULL);
}

void ObjectConfigData::destroyConfigService() {

    if (_clientWorker != NULL) {
        _clientWorker->stopListening();
    }

    if (_securityManager != NULL) {
        _securityManager->stop();
    }

    if (_serverListManager) {
        _serverListManager->stop();
    }

    if (_clientWorker != NULL) {
        _clientWorker->stopListening();
        delete _clientWorker;
        _clientWorker = NULL;
    }

    if (_httpDelegate != NULL) {
        delete _httpDelegate;
        _httpDelegate = NULL;
    }

    if (_securityManager != NULL) {
        delete _securityManager;
        _securityManager = NULL;
    }

    if (_serverListManager != NULL) {
        delete _serverListManager;
        _serverListManager = NULL;
    }

    if (_httpCli != NULL) {
        delete _httpCli;
        _httpCli = NULL;
    }

    if (_appConfigManager != NULL) {
        delete _appConfigManager;
        _appConfigManager = NULL;
    }

    if (_configProxy != NULL) {
        delete _configProxy;
        _configProxy = NULL;
    }
}

void ObjectConfigData::destroyNamingService() {

    if (_beatReactor != NULL) {
        _beatReactor->stop();
    }

    if (_subscriptionPoller != NULL) {
        _subscriptionPoller->stop();
    }

    if (_udpNamingServiceListener != NULL) {
        _udpNamingServiceListener->stop();
    }

    if (_eventDispatcher != NULL) {
        _eventDispatcher->stop();
    }

    if (_securityManager != NULL) {
        _securityManager->stop();
    }

    if (_serverListManager) {
        _serverListManager->stop();
    }

    if (_httpDelegate != NULL) {
        delete _httpDelegate;
        _httpDelegate = NULL;
    }

    if (_beatReactor != NULL) {
        delete _beatReactor;
        _beatReactor = NULL;
    }

    if (_subscriptionPoller != NULL)
    {
        delete _subscriptionPoller;
        _subscriptionPoller = NULL;
    }

    if (_udpNamingServiceListener != NULL)
    {
        delete _udpNamingServiceListener;
        _udpNamingServiceListener = NULL;
    }

    if (_eventDispatcher != NULL)
    {
        delete _eventDispatcher;
        _eventDispatcher = NULL;
    }

    if (_hostReactor != NULL) {
        delete _hostReactor;
        _hostReactor = NULL;
    }

    if (_serverProxy != NULL) {
        delete _serverProxy;
        _serverProxy = NULL;
    }

    if (_securityManager != NULL) {
        delete _securityManager;
        _securityManager = NULL;
    }

    if (_serverListManager != NULL) {
        delete _serverListManager;
        _serverListManager = NULL;
    }

    if (_httpDelegate != NULL) {
        delete _httpDelegate;
        _httpDelegate = NULL;
    }

    if (_httpCli != NULL) {
        delete _httpCli;
        _httpCli = NULL;
    }

    if (_appConfigManager != NULL)
    {
        delete _appConfigManager;
        _appConfigManager = NULL;
    }

    if (_sequenceProvider != NULL)
    {
        delete _sequenceProvider;
        _sequenceProvider = NULL;
    }
}

void ObjectConfigData::destroyMaintainService() {
    if (_serverListManager != NULL) {
        _serverListManager->stop();
    }

    if (_securityManager != NULL) {
        _securityManager->stop();
    }

    if (_serverProxy != NULL) {
        delete _serverProxy;
        _serverProxy = NULL;
    }
    if (_serverListManager != NULL) {
        delete _serverListManager;
        _serverListManager = NULL;
    }
    if (_appConfigManager != NULL) {
        delete _appConfigManager;
        _appConfigManager = NULL;
    }
    if (_securityManager != NULL) {
        delete _securityManager;
        _securityManager = NULL;
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

void ObjectConfigData::checkAssembledObject() NACOS_THROW(NacosException) {
    switch (factoryType) {
    case NAMING:
        checkNamingService();
        return;
    case CONFIG:
        checkConfigService();
        return;
    case MAINTAIN:
        checkMaintainService();
        break;
    default:
        abort();//never happens
    }
}

ObjectConfigData::~ObjectConfigData() {
    switch (factoryType) {
    case NAMING:
        destroyNamingService();
        return;
    case CONFIG:
        destroyConfigService();
        return;
    case MAINTAIN:
        destroyMaintainService();
        break;
    default:
        abort();//never happens
    }
}

}//namespace nacos
