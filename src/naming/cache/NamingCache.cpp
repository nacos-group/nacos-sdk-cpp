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
    namingList[key] = info;
}