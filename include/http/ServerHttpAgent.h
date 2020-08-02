#ifndef __SVR_HTTP_AGENT_H_
#define __SVR_HTTP_AGENT_H_
#include "NacosExceptions.h"
#include "NacosString.h"
#include "http/HTTPCli.h"
#include "http/HttpAgent.h"
#include "ServerListManager.h"

/**
 * ServerHttpAgent
 *
 * @author Liu, Hanyu
 */
class ServerHttpAgent : public HttpAgent
{
private:
	//Variables
	NacosString namesp;
	NacosString encoding;
	HTTPCli *httpCli = NULL;
	ServerListManager *svrListMgr = NULL;
public:
	ServerHttpAgent(HTTPCli *httpcli, const NacosString &_encoding, ServerListManager *_svrListMgr, const NacosString &_namesp);
	ServerHttpAgent(HTTPCli *httpcli, const NacosString &encoding, ServerListManager *_svrListMgr);
	HttpResult httpGet(const NacosString &path, std::list<NacosString> &headers, std::list<NacosString> &paramValues, const NacosString &encoding, long readTimeoutMs) throw (NetworkException);
	HttpResult httpPost(const NacosString &path, std::list<NacosString> &headers, std::list<NacosString> &paramValues, const NacosString &encoding, long readTimeoutMs) throw (NetworkException);
	HttpResult httpDelete(const NacosString &path, std::list<NacosString> &headers, std::list<NacosString> &paramValues, const NacosString &encoding, long readTimeoutMs) throw (NetworkException);
	NacosString getEncode() const;
	virtual ~ServerHttpAgent() { httpCli = NULL; svrListMgr = NULL; };
};

#endif