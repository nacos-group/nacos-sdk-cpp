#ifndef __NAMING_PROXY_H_
#define __NAMING_PROXY_H_

#include <map>
#include <list>
#include "NacosString.h"
#include "NacosExceptions.h"
#include "http/HTTPCli.h"
#include "naming/Instance.h"
#include "naming/beat/BeatInfo.h"
#include "server/ServerListManager.h"
#include "naming/ListView.h"

class NamingProxy {
private:
    NacosString serverPort;
    HTTPCli *httpCli = NULL;
    ServerListManager *serverListManager;
    AppConfigManager *appConfigManager;

    NacosString nacosDomain;

    NamingProxy();

    NacosString
    reqAPI(const NacosString &api, std::map <NacosString, NacosString> &params, int method) throw(NacosException);

    NacosString
    callServer(const NacosString &api, std::map <NacosString, NacosString> &params, const NacosString &curServer,
               int method) throw(NacosException);

    NacosString callServer(const NacosString &api, std::map <NacosString, NacosString> &params,
                           const NacosString &curServer) throw(NacosException);

    std::list <NacosString> builderHeaders();

    long _http_req_timeout;
    long _hb_fail_wait;//Time to wait when a heartbeat request fails (in ms)
public:
    NamingProxy(HTTPCli *httpcli, ServerListManager *serverListManager, AppConfigManager *appConfigManager);

    ~NamingProxy();

    void registerService(const NacosString &serviceName, const NacosString &groupName,
                         Instance &instance) throw(NacosException);

    void deregisterService(const NacosString &serviceName, Instance &instance) throw(NacosException);

    NacosString queryList(const NacosString &serviceName, const NacosString &clusters, int udpPort,
                          bool healthyOnly) throw(NacosException);

    ListView<NacosString> getServiceList(int page, int pageSize, const NacosString &groupName) throw(NacosException);

    inline NacosString getNamespaceId();

    long sendBeat(BeatInfo &beatInfo);
};

#endif
