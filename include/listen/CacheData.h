#ifndef __CACHEDATA_H_
#define __CACHEDATA_H_
#include <map>
#include <pthread.h>
#include "NacosString.h"
#include "http/HttpAgent.h"
#include "listen/Listener.h"

//We keep it simple first, only 1 watcher thread and only 1 listener for 1 key
class Cachedata
{
public:
	NacosString tenant;
	NacosString dataId;
	NacosString group;
	NacosString dataMD5;
	Listener *listener;
	bool operator == (const Cachedata &rhs) const
	{
		return tenant == rhs.tenant && dataId == rhs.dataId && group == rhs.group;
	};
};

#endif