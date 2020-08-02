#ifndef __NACOS_CFG_SVC_H_
#define __NACOS_CFG_SVC_H_
#include "config/ConfigService.h"
#include "http/HTTPAgent.h"
#include "listen/ClientWorker.h"
#include "NacosString.h"
#include "ServerListManager.h"
#include "Properties.h"

class NacosConfigService : public ConfigService
{
private:
	//Variables
	NacosString namesp;
	NacosString encoding;
	HttpAgent *httpAgent = NULL;
	HTTPCli *httpcli = NULL;
	ServerListManager *svrListMgr = NULL;
	ClientWorker *clientWorker = NULL;

	//Private Methods
	NacosConfigService();
    NacosString getConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group, long timeoutMs) throw(NacosException);
	bool removeConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group, const NacosString &tag) throw(NacosException);
	bool publishConfigInner(const NacosString &tenant, const NacosString &dataId, const NacosString &group, const NacosString &tag, const NacosString &appName, const NacosString &betaIps, const NacosString &content) throw (NacosException);
	//NacosString monitorChange(std::map<NacosString, NacosString> &keysAndContents, long timeoutMs) throw (NacosException);
	//static NacosString monitorListToString(std::map<NacosString, NacosString> &keysAndContents);
public:
	const static long POST_TIMEOUT = 3000L;
	//Public Methods
	NacosString getConfig(const NacosString &dataId, const NacosString &group, long timeoutMs) throw (NacosException);
	bool publishConfig(const NacosString &dataId, const NacosString &group, const NacosString &content) throw (NacosException);
	bool removeConfig(const NacosString &dataId, const NacosString &group) throw(NacosException);
	void addListener(const NacosString &dataId, const NacosString &group, Listener *listener) throw(NacosException);
	//ctors/dtor
	NacosConfigService(Properties &props) throw (NacosException);
	virtual ~NacosConfigService();
};

#endif