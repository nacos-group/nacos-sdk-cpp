#ifndef __SERVER_LIST_MGR_H_
#define __SERVER_LIST_MGR_H_
#include <list>
#include "NacosString.h"
#include "Properties.h"
#include "NacosExceptions.h"

class ServerListManager
{
private:
	std::list<NacosString> serverList;
	NacosString contentPath;
	void initParams();
	void initSrvListWithAddress(NacosString &address);
public:
	ServerListManager(std::list<NacosString> &fixed);
	ServerListManager(Properties &props) throw(NacosException);
	NacosString getContentPath() { return contentPath; };
	NacosString getCurrentServerAddr();
};
#endif