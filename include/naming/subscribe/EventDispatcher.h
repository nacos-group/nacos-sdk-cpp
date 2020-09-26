#ifndef __EVT_DISPTCH_H_
#define __EVT_DISPTCH_H_

#include "thread/Thread.h"
#include "thread/RWLock.h"
#include "thread/BlockingQueue.h"
#include "EventListener.h"
#include <map>

struct NotifyData
{
    bool exit = false;
    ChangeAdvice changeAdvice;
    std::list<EventListener *> listeners;
};

class EventDispatcher {
private:
    volatile bool _started = false;
    RWLock rwLock;//for observerMap
    std::map<NacosString, std::list<EventListener*> > observerMap;
    BlockingQueue<NotifyData> notifyDataList = BlockingQueue<NotifyData>(1000);
    Thread *eventNotifier = NULL;

    static void *eventDispatcherThread(void *parm);

    void purgeAllListeners();
public:
    bool addListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener);
    bool removeListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener);
    void notify(const ChangeAdvice &changeAdvice);
    void stop();
    void start();
    EventDispatcher();
    ~EventDispatcher();
};

#endif