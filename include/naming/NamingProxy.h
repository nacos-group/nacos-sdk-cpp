#ifndef __NAMING_PROXY_H_
#define __NAMING_PROXY_H_
#include <map>
#include <list>
#include "NacosString.h"
#include "NacosExceptions.h"
#include "http/HTTPCli.h"
#include "naming/Instance.h"
#include "naming/beat/BeatInfo.h"

class NamingProxy
{
private:
	HTTPCli *httpCli = NULL;
	NamingProxy();
    NacosString reqAPI(const NacosString &api, std::map<NacosString, NacosString> &params, int method) throw (NacosException);
	NacosString reqAPI(const NacosString &api, std::map<NacosString, NacosString> &params, std::list<NacosString> &servers, int method) throw (NacosException);
	NacosString callServer(const NacosString &api, std::map<NacosString, NacosString> &params, const NacosString &curServer, int method) throw (NacosException);
	NacosString callServer(const NacosString &api, std::map<NacosString, NacosString> &params, const NacosString &curServer) throw (NacosException);
	std::list<NacosString> builderHeaders();
	NacosString serverPort;
	NacosString nacosDomain;
	NacosString namespaceId;
	NacosString endpoint;
	std::list<NacosString> serverList;
public:
	NamingProxy(HTTPCli *httpcli, const NacosString &namespaceId, const NacosString &endpoint, const NacosString &serverList);
	~NamingProxy();
	void registerService(const NacosString &serviceName, const NacosString &groupName, Instance &instance) throw (NacosException);
	void deregisterService(const NacosString &serviceName, Instance &instance) throw (NacosException);
    NacosString queryList(const NacosString &serviceName, const NacosString &clusters, int udpPort, bool healthyOnly) throw (NacosException);
	NacosString getNamespaceId();
	long sendBeat(BeatInfo &beatInfo);
};

#endif
