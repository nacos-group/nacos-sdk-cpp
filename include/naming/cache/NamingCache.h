#ifndef __NAMING_CACHE_H_
#define __NAMING_CACHE_H_

#include <map>
#include "NacosString.h"
#include "naming/ServiceInfo.h"
#include "thread/RWLock.h"
#include "ChangeAdvice.h"

class NamingCache {
private:
    std::map<NacosString, ServiceInfo> namingList;
    RWLock _rwLock;
public:
    ServiceInfo getServiceInfo(const NacosString &key) throw(NacosException);
    bool contains(const NacosString &key);
    void setServiceInfo(const NacosString &key, const ServiceInfo &info);
};

#endif