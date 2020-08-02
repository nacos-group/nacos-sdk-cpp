#include <map>
#include "naming/beat/BeatReactor.h"
#include "naming/beat/BeatTask.h"
#include "NacosString.h"
#include "DebugAssertion.h"

using namespace std;

void BeatReactor::start()
{
	_stop = false;
	_beatMaster->start();
	_threadPool->start();
}

void BeatReactor::stop()
{
	if (_stop)
	{
		log_warn("Calling BeatReactor::stop() on an already-stopped BeatReactor\n");
		return;
	}
	_stop = true;
	_beatMaster->join();
	_threadPool->stop();
}


void *BeatReactor::beatMaster(void *param)
{
	BeatReactor *thisobj = (BeatReactor*)param;
	while (!thisobj->_stop)
	{
		{
		LockGuard _lockguard(thisobj->_beatInfoLock);
		for (map<NacosString, BeatTask*>::iterator it = thisobj->_beatInfoList.begin();
			it != thisobj->_beatInfoList.end(); it++)
		{
			BeatTask *curtask = it->second;
			if (curtask->getScheduled())
			{
				continue;
			}
			curtask->incRef();
			curtask->setScheduled(true);
			thisobj->_threadPool->put(curtask);
		}
		}
		sleep(thisobj->_clientBeatInterval / 1000);//TODO:sleep in a more precise way
	}

	return NULL;
}

void BeatReactor::addBeatInfo(const NacosString &serviceName, BeatInfo &beatInfo)
{
	NacosString beatInfoStr = beatInfo.toString();
	log_info("[BEAT] adding beat: %s to beat map.", beatInfoStr.c_str());
	BeatTask *beattask = new BeatTask(beatInfo, _namingProxy, this);
	NacosString beatKey = buildKey(serviceName, beatInfo.ip, beatInfo.port);
	beattask->setTaskName(beatKey);
	{
		LockGuard _lockguard(_beatInfoLock);
		//The specified beatInfo is already in the list
		if (_beatInfoList.count(beatKey) != 0)
		{
			log_warn("Adding already-exist key:%s\n", beatKey.c_str());
			return;
		}
		_beatInfoList[beatKey] = beattask;
	}
	//TODO:MetricsMonitor.getDom2BeatSizeMonitor().set(dom2Beat.size());
}

void BeatReactor::removeBeatInfo(const NacosString &serviceName, const NacosString &ip, int port)
{
	log_info("[BEAT] removing beat: %s:%s:%d from beat map.", serviceName.c_str(), ip.c_str(), port);
	NacosString beatKey = buildKey(serviceName, ip, port);
	BeatTask *beattask = NULL;
	{
		LockGuard _lockguard(_beatInfoLock);
		//If we can't find the beatInfo in the list, just return
		if (_beatInfoList.count(beatKey) != 1)
		{
			log_warn("Removing non-existent key:%s\n", beatKey.c_str());
			return;
		}
		beattask = _beatInfoList[beatKey];
		_beatInfoList.erase(beatKey);
	}
	int refcount = beattask->decRef();
	if (refcount == 0)
	{
		log_debug("[BEAT]:refCount is 0, deleting the object\n");
		delete beattask;
	}
	//TODO:MetricsMonitor.getDom2BeatSizeMonitor().set(dom2Beat.size());
}

void BeatReactor::removeAllBeatInfo()
{
	LockGuard _lockguard(_beatInfoLock);
	for (map<NacosString, BeatTask*>::iterator it = _beatInfoList.begin();
		it != _beatInfoList.end(); it++)
	{
		BeatTask *curtask = it->second;
		NACOS_ASSERT(curtask->getRef() == 1);
		curtask->decRef();
		delete curtask;
		curtask = NULL;
	}
	_beatInfoList.clear();
}

NacosString BeatReactor::buildKey(const NacosString &serviceName, const NacosString &ip, int port)
{
	return serviceName + Constants::NAMING_INSTANCE_ID_SPLITTER
	+ ip + Constants::NAMING_INSTANCE_ID_SPLITTER + NacosStringOps::valueOf(port);
}