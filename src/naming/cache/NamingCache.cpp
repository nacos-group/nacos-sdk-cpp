#include "naming/cache/NamingCache.h"

ServiceInfo NamingCache::getServiceInfo(const NacosString &key) throw(NacosException)
{
    ReadGuard __readGuard(_rwLock);
    if (namingList.count(key) == 0)
    {
        throw NacosException(0, "Key" + key + " doesn't exist");
    }
    return namingList[key];
}

bool NamingCache::contains(const NacosString &key)
{
    ReadGuard __readGuard(_rwLock);
    return namingList.count(key) > 0;
}

void NamingCache::setServiceInfo(const NacosString &key, const ServiceInfo &info)
{
    WriteGuard __writeGuard(_rwLock);
    if (_eventDispatcher != NULL)
    {
        ChangeAdvice changeAdvice;
        if (namingList.count(key) > 0)
        {
            changeAdvice.changeType = ChangeType::UPDATE;
            changeAdvice.oldServiceInfo = namingList[key];
            changeAdvice.newServiceInfo = info;
        }
        else
        {
            changeAdvice.changeType = ChangeType::ADD;
            changeAdvice.newServiceInfo = info;
        }
        _eventDispatcher->notify(changeAdvice);
    }
    namingList[key] = info;
}

void NamingCache::removeServiceInfo(const NacosString &key)
{
    WriteGuard __writeGuard(_rwLock);
    if (namingList.count(key) == 0)
    {
        return;
    }
    if (_eventDispatcher != NULL)
    {
        ChangeAdvice changeAdvice;
        changeAdvice.changeType = ChangeType::REMOVE;
        changeAdvice.oldServiceInfo = namingList[key];
        _eventDispatcher->notify(changeAdvice);
    }
    namingList.erase(key);
}