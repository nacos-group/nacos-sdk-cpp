#ifndef __CLIENT_WORKER_H_
#define __CLIENT_WORKER_H_
#include <map>
#include <vector>
#include <pthread.h>
#include "NacosString.h"
#include "http/HttpAgent.h"
#include "listen/Listener.h"
#include "listen/CacheData.h"
#include "NacosExceptions.h"

class ClientWorker
{
private:
	//dataID||group||tenant -> Cachedata* Mapping
	std::map<NacosString, Cachedata*> watchList;
	pthread_mutex_t watchListMutex;
	HttpAgent *httpAgent = NULL;
	//Listener thread related info
	pthread_t threadId;

	bool stopThread;
	pthread_mutex_t stopThreadMutex;

	static void *listenerThread(void *watcher);

	//You just can't construct a ClientWorker object without any parameter
	ClientWorker();
	std::vector<NacosString> parseListenedKeys(const NacosString &ReturnedKeys);
	NacosString checkListenedKeys();
public:
	ClientWorker(HttpAgent *_httpAgent);
	~ClientWorker();
	void startListening();
	void stopListening();
	void addListener(const Cachedata &cachedata);
	void removeListener(const Cachedata &cachedata);
	void performWatch();
	NacosString getServerConfig(const NacosString &tenant, const NacosString &dataId, const NacosString &group, long timeoutMs) throw (NacosException);
};

#endif