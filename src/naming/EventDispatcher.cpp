//
// Created by liuhanyu on 2020/9/25.
//

#include "naming/subscribe/EventDispatcher.h"
#include "DebugAssertion.h"

using namespace std;

bool EventDispatcher::addListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener)
{
    WriteGuard __writeGuard(rwLock);
    NacosString key = ServiceInfo::getKey(serviceName, clusters);
    if (observerMap.count(key) > 0)
    {
        list<EventListener*> listenerList = observerMap[key];
        for (auto i : listenerList)
        {
            if (i == eventListener)
            {
                return false;
            }
        }
        eventListener->incRef();
        listenerList.push_back(eventListener);
        return true;
    }
    else
    {
        list<EventListener*> listenerList;
        eventListener->incRef();
        listenerList.push_back(eventListener);
        observerMap[key] = listenerList;
        return true;
    }
}

bool EventDispatcher::removeListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener)
{
    WriteGuard __writeGuard(rwLock);
    NacosString key = ServiceInfo::getKey(serviceName, clusters);
    if (observerMap.count(key) == 0) {
        return false;
    }
    else
    {
        list<EventListener*> listenerList = observerMap[key];
        for (list<EventListener*>::iterator it = listenerList.begin();
            it != listenerList.end(); it++) {
            if (*it == eventListener) {
                EventListener *removedItem = *it;
                listenerList.erase(it);

                int refCount = removedItem->decRef();
                if (refCount == 0) {
                    delete removedItem;
                }
                if (listenerList.empty()) {
                    observerMap.erase(key);
                }
                return true;
            }
        }
        return false;
    }
}

void EventDispatcher::start()
{
    if (_started) {
        return;
    }
    _started = true;
    eventNotifier->start();
}

void EventDispatcher::stop()
{
    if (!_started) {
        log_debug("Calling stop() on a already-stopped EventDispatcher\n");
        return;
    }
    _started = false;
    NotifyData notifyData;
    notifyData.exit = true;
    notifyDataList.enqueue(notifyData);
}

void EventDispatcher::notify(const ChangeAdvice &changeAdvice)
{
    ReadGuard __readGuard(rwLock);

    NacosString key;
    switch (changeAdvice.changeType) {
    case ADD:
        key = changeAdvice.newServiceInfo.getKey();
        break;
    case UPDATE:
        key = changeAdvice.oldServiceInfo.getKey();
        break;
    case REMOVE:
        key = changeAdvice.oldServiceInfo.getKey();
        break;
    }
    if (observerMap.count(key) == 0)
    {
        log_debug("Notifying non-existent key:%s\n", key.c_str());
        return;
    }

    list<EventListener*> listeners = observerMap[key];
    NotifyData notifyData;
    notifyData.listeners = listeners;
    notifyData.changeAdvice = changeAdvice;
    for (auto i : listeners) {
        i->incRef();
    }
    notifyDataList.enqueue(notifyData);
}

void *EventDispatcher::eventDispatcherThread(void *parm)
{
    EventDispatcher *thisObj = (EventDispatcher*)parm;

    while (true) {
        NotifyData notifyData = thisObj->notifyDataList.dequeue();

        if (notifyData.exit) {
            while (!thisObj->notifyDataList.empty()) {
                NotifyData notifyDataToRemove = thisObj->notifyDataList.dequeue();
                for (auto curListener : notifyDataToRemove.listeners) {
                    curListener->decRef();
                    int refCount = curListener->decRef();
                    if (refCount == 0) {
                        delete curListener;
                    }
                }
            }
            return 0;
        }

        for (auto curListener : notifyData.listeners) {
            curListener->receiveNamingInfo(notifyData.changeAdvice);
            curListener->decRef();
            int refCount = curListener->decRef();
            if (refCount == 0) {
                delete curListener;
            }
        }
    }

    return 0;
}

//NOTE:This function can be called ONLY when the dispatcher thread is stopped!
void EventDispatcher::purgeAllListeners()
{
    WriteGuard __writeGuard(rwLock);

    for (auto i : observerMap) {
        for (auto listenerItem : i.second) {
            int refCnt = listenerItem->decRef();
            NACOS_ASSERT(refCnt == 0);
            delete listenerItem;
        }
    }

    observerMap.clear();
}

EventDispatcher::EventDispatcher()
{
    eventNotifier = new Thread("Nacos-NamingEvent-Dispatcher", eventDispatcherThread, (void*)this);
}

EventDispatcher::~EventDispatcher()
{
    stop();
    eventNotifier->join();
    delete eventNotifier;
    eventNotifier = NULL;
}