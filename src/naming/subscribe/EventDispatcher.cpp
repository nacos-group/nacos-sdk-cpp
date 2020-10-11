//
// Created by liuhanyu on 2020/9/25.
//

#include "naming/subscribe/EventDispatcher.h"
#include "DebugAssertion.h"

using namespace std;

bool EventDispatcher::addListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener)
{
    NACOS_ASSERT(eventListener != NULL);
    WriteGuard __writeGuard(rwLock);
    NacosString key = ServiceInfo::getKey(serviceName, clusters);
    if (observerMap.count(key) > 0)
    {
        list<EventListener*> &listenerList = observerMap[key];
        for (list<EventListener*>::iterator it = listenerList.begin();
            it != listenerList.end(); it++)
        {
            if ((*it) == eventListener)
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

bool EventDispatcher::removeListenerHelper(const NacosString &key, EventListener *eventListener, int &remainingListeners)
{
    list<EventListener*> &listenerList = observerMap[key];
    for (list<EventListener*>::iterator it = listenerList.begin();
         it != listenerList.end(); it++) {
        if (*it == eventListener) {
            EventListener *removedItem = *it;
            log_debug("erased item: %s\n", removedItem->getListenerName().c_str());
            listenerList.erase(it);

            int refCount = removedItem->decRef();
            if (refCount == 0) {
                log_debug("The refcount of listener '%s' is 0, deleting the item.\n", removedItem->getListenerName().c_str());
                delete removedItem;
            }
            if (listenerList.empty()) {
                observerMap.erase(key);
            }
            remainingListeners = listenerList.size();
            return true;
        }
    }
    return false;
}

bool EventDispatcher::removeListener(const NacosString &serviceName, const NacosString &clusters, EventListener *eventListener, int &remainingListeners)
{
    NACOS_ASSERT(eventListener != NULL);
    WriteGuard __writeGuard(rwLock);
    NacosString key = ServiceInfo::getKey(serviceName, clusters);
    if (observerMap.count(key) == 0) {
        remainingListeners = 0;
        return false;
    }
    else
    {
        return removeListenerHelper(key, eventListener, remainingListeners);
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
    NacosString key = changeAdvice.key;

    NotifyData notifyData;
    {
        ReadGuard __readGuard(rwLock);
        if (observerMap.count(key) == 0)
        {
            log_debug("Notifying non-existent key:%s\n", key.c_str());
            return;
        }
        list<EventListener*> &listeners = observerMap[key];
        for (list<EventListener *>::iterator it = listeners.begin();
            it != listeners.end(); it++) {
            (*it)->incRef();
        }
        notifyData.listeners = listeners;
    }
    notifyData.serviceInfo = changeAdvice.newServiceInfo;
    notifyDataList.enqueue(notifyData);
}

void EventDispatcher::notifyDirectly(const ChangeAdvice &changeAdvice)
{
    NacosString key = changeAdvice.key;

    list<EventListener*> listeners;
    {
        ReadGuard __readGuard(rwLock);
        if (observerMap.count(key) == 0)
        {
            log_debug("Notifying non-existent key:%s\n", key.c_str());
            return;
        }
        listeners = observerMap[key];
        for (list<EventListener*>::iterator it = listeners.begin();
            it != listeners.end(); it++) {
            (*it)->incRef();
        }
    }

    for (list<EventListener*>::iterator curListener = listeners.begin();
        curListener != listeners.end(); curListener++)
    {
        //asm volatile("int $3");
        (*curListener)->receiveNamingInfo(changeAdvice.newServiceInfo);
        int refCnt = (*curListener)->decRef();
        if (refCnt == 0) {
            log_debug("Destroying listener whose refcount = 0\n");
            delete *curListener;
        }
    }
}

void *EventDispatcher::eventDispatcherThread(void *parm)
{
    EventDispatcher *thisObj = (EventDispatcher*)parm;

    while (true) {
        NotifyData notifyData = thisObj->notifyDataList.dequeue();

        if (notifyData.exit) {
            while (!thisObj->notifyDataList.empty()) {
                NotifyData notifyDataToRemove = thisObj->notifyDataList.dequeue();
                for (list<EventListener*>::iterator curListener = notifyDataToRemove.listeners.begin();
                curListener != notifyDataToRemove.listeners.end(); curListener++) {
                    (*curListener)->decRef();
                    int refCount = (*curListener)->decRef();
                    if (refCount == 0) {
                        delete (*curListener);
                    }
                }
            }
            return 0;
        }

        for (list<EventListener*>::iterator curListener = notifyData.listeners.begin();
            curListener != notifyData.listeners.end(); curListener++) {
            (*curListener)->receiveNamingInfo(notifyData.serviceInfo);
            (*curListener)->decRef();
            int refCount = (*curListener)->decRef();
            if (refCount == 0) {
                delete (*curListener);
            }
        }
    }

    return 0;
}

//NOTE:This function can be called ONLY when the dispatcher thread is stopped!
//This designed to be called when the an EventDispatcher Object is dtor'd
void EventDispatcher::purgeAllListeners()
{
    WriteGuard __writeGuard(rwLock);

    for (map<NacosString, list<EventListener*> >::iterator it = observerMap.begin();
            it != observerMap.end(); it++) {

        for (list<EventListener*>::iterator listenerItem = it->second.begin();
            listenerItem != it->second.end(); listenerItem++) {
            int refCount = (*listenerItem)->decRef();
            if (refCount == 0)
            {
                log_debug("Deleting the object whose refcount = 0:%s\n", (*listenerItem)->getListenerName().c_str());
                delete *listenerItem;
                *listenerItem = NULL;
            }
        }
    }

    for (map<NacosString, list<EventListener*> >::iterator it = observerMap.begin();
         it != observerMap.end(); it++) {
        for (list<EventListener*>::iterator curListener = it->second.begin();
            curListener != it->second.end(); curListener++)
        {
            NACOS_ASSERT(*curListener == NULL);
        }
    }

    observerMap.clear();
}

EventDispatcher::EventDispatcher()
{
    _started = false;
    eventNotifier = new Thread("Nacos-NamingEvent-Dispatcher", eventDispatcherThread, (void*)this);
}

EventDispatcher::~EventDispatcher()
{
    stop();
    eventNotifier->join();
    delete eventNotifier;
    eventNotifier = NULL;
    purgeAllListeners();
}