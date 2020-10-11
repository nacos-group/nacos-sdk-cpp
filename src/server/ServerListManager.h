#ifndef __SERVER_LIST_MGR_H_
#define __SERVER_LIST_MGR_H_

#include <list>
#include "NacosString.h"
#include "Properties.h"
#include "NacosExceptions.h"
#include "NacosServerInfo.h"
#include "http/HTTPCli.h"
#include "thread/Thread.h"
#include "config/AppConfigManager.h"
#include "PropertyKeyConst.h"
#include "thread/RWLock.h"

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
    Thread *_pullThread = NULL;

    static void *pullWorkerThread(void *param);

    long refreshInterval;//in Millis
    RWLock rwLock;//to lock the serverList

    void initAll() throw(NacosException);

    void addToSrvList(NacosString &address);

    std::list <NacosServerInfo> tryPullServerListFromNacosServer() throw(NacosException);

    std::list <NacosServerInfo> pullServerList() throw(NacosException);

    HTTPCli *httpCli = NULL;
    AppConfigManager *appConfigManager = NULL;

    static NacosString serverListToString(const std::list <NacosServerInfo> &serverList);

    long _read_timeout;

public:
    //Cluster info
    inline NacosString getClusterName() const { return appConfigManager->get(PropertyKeyConst::CLUSTER_NAME); };

    inline NacosString getEndpoint() const { return appConfigManager->get(PropertyKeyConst::ENDPOINT); };

    inline int getEndpointPort() const { return atoi(appConfigManager->get(PropertyKeyConst::ENDPOINT_PORT).c_str()); };

    inline NacosString getContextPath() const;

    inline NacosString getNamespace() const { return appConfigManager->get(PropertyKeyConst::NAMESPACE); };

    std::list <NacosServerInfo> __debug();//DO NOT use, may be changed without prior notification

    HTTPCli *getHttpCli() const { return httpCli; };

    void setHttpCli(HTTPCli *_httpCli) { this->httpCli = _httpCli; };

    AppConfigManager *getAppConfigManager() const { return appConfigManager; };

    void setAppConfigManager(AppConfigManager *_appConfigManager) { this->appConfigManager = _appConfigManager; };

    ServerListManager(std::list <NacosString> &fixed);

    ServerListManager(HTTPCli *_httpCli, AppConfigManager *_appConfigManager) throw(NacosException);

    NacosString getCurrentServerAddr();

    int getServerCount();

    std::list <NacosServerInfo> getServerList();

    NacosString toString() const;

    void start();

    void stop();

    ~ServerListManager();
};

#endif