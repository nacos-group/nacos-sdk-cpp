#ifndef __EVT_DISPTCH_H_
#define __EVT_DISPTCH_H_

#include "thread/Thread.h"
#include "thread/RWLock.h"
#include "thread/BlockingQueue.h"
#include "EventListener.h"
#include <map>

struct NotifyData
{
    NotifyData() { exit = false; };
    bool exit;
    ServiceInfo serviceInfo;
    std::list<EventListener *> listeners;
};

//TODO:refactor to 2 types of eventDispatcher:
//1. blocking mode, notify in notifier's thread
//2. non-blocking mode(async), dispatcher has a blocking queue and send notify in that thread asynchronously
class EventDispatcher {
private:
    volatile bool _started;
    RWLock rwLock;//for observerMap
    std::map<NacosString, std::list<EventListener*> > observerMap;
    BlockingQueue<NotifyData> notifyDataList;
    Thread *eventNotifier = NULL;

    static void *eventDispatcherThread(void *parm);

    bool removeListenerHelper(const NacosString &key, EventListener *eventListener, int &remainingListeners);

    void purgeAllListeners();
public:
    bool addListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener);
    bool removeListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener, int &remainingListeners);
    void notify(const ChangeAdvice &changeAdvice);
    void notifyDirectly(const ChangeAdvice &changeAdvice);
    void stop();
    void start();
    EventDispatcher();
    ~EventDispatcher();
};

#endif