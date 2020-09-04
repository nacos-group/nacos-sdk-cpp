#ifndef __SERVER_LIST_MGR_H_
#define __SERVER_LIST_MGR_H_
#include <list>
#include "NacosString.h"
#include "Properties.h"
#include "NacosExceptions.h"
#include "NacosServerInfo.h"
#include "http/HTTPCli.h"

class ServerListManager
{
private:
    //Fixed serverList, only keep the servers loaded from the config file
	std::list<NacosString> serverList;
	NacosString contentPath;
	void initParams();
	void initSrvListWithAddress(NacosString &address);
	std::map<NacosString, NacosServerInfo> getServerList();
	HTTPCli *httpCli = NULL;
public:
    std::map<NacosString, NacosServerInfo> __debug();

    HTTPCli * getHttpCli() const { return httpCli; };
    void setHttpCli(HTTPCli *_httpCli) { this->httpCli = _httpCli; };
	ServerListManager(std::list<NacosString> &fixed);
	ServerListManager(Properties &props) throw(NacosException);
	NacosString getContentPath() { return contentPath; };
	NacosString getCurrentServerAddr();
};
#endif