#ifndef __SERVER_LIST_MGR_H_
#define __SERVER_LIST_MGR_H_

#include <list>
#include "NacosString.h"
#include "Properties.h"
#include "NacosExceptions.h"
#include "src/server/NacosServerInfo.h"
#include "src/http/HttpDelegate.h"
#include "src/thread/Thread.h"
#include "src/config/AppConfigManager.h"
#include "constant/PropertyKeyConst.h"
#include "src/thread/RWLock.h"
#include "src/factory/ObjectConfigData.h"
#include "Compatibility.h"

namespace nacos{
class ServerListManager {
private:
    //status info
    bool started;
    bool isFixed;

    //Nacos server info
    std::list <NacosServerInfo> serverList;

    //url to pull nacos cluster nodes info from
    NacosString addressServerUrl;

    //Worker thread
    Thread *_pullThread;

    static void *pullWorkerThread(void *param);

    long refreshInterval;//in Millis
    RWLock rwLock;//to lock the serverList

    void initAll() NACOS_THROW(NacosException);

    void addToSrvList(NacosString &address);

    std::list <NacosServerInfo> tryPullServerListFromNacosServer() NACOS_THROW(NacosException);

    std::list <NacosServerInfo> pullServerList() NACOS_THROW(NacosException);

    ObjectConfigData *_objectConfigData;

    static NacosString serverListToString(const std::list <NacosServerInfo> &serverList);

public:
    //Cluster info
    inline const NacosString& getClusterName() { return _objectConfigData->_appConfigManager->get(PropertyKeyConst::CLUSTER_NAME); };

    inline const NacosString& getEndpoint() { return _objectConfigData->_appConfigManager->get(PropertyKeyConst::ENDPOINT); };

    inline int getEndpointPort() { return atoi(_objectConfigData->_appConfigManager->get(PropertyKeyConst::ENDPOINT_PORT).c_str()); };

    inline const NacosString &getContextPath() const;

    inline const NacosString& getEndpointContextPath() { return  _objectConfigData->_appConfigManager->get(PropertyKeyConst::ENDPOINT_CONTEXT_PATH); }

    inline const NacosString& getNamespace() { return _objectConfigData->_appConfigManager->get(PropertyKeyConst::NAMESPACE); };

    inline const NacosString& getEndpointQueryParams() { return _objectConfigData->_appConfigManager->get(PropertyKeyConst::ENDPOINT_QUERY_PARAMS); }

    std::list <NacosServerInfo> __debug();//DO NOT use, may be changed without prior notification

    HttpDelegate *getHttpDelegate() const { return _objectConfigData->_httpDelegate; };

    void setHttpDelegate(HttpDelegate *httpDelegate) { _objectConfigData->_httpDelegate = httpDelegate; };

    AppConfigManager *getAppConfigManager() const { return _objectConfigData->_appConfigManager; };

    void setAppConfigManager(AppConfigManager *_appConfigManager) { _objectConfigData->_appConfigManager = _appConfigManager; };

    ServerListManager(std::list <NacosString> &fixed);

    ServerListManager(ObjectConfigData *objectConfigData) NACOS_THROW(NacosException);

    NacosString getCurrentServerAddr();

    int getServerCount();

    std::list <NacosServerInfo> getServerList();

    NacosString toString() const;

    void start();

    void stop();

    ~ServerListManager();
};
}//namespace nacos

#endif