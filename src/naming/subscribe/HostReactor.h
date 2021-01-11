//
// Created by liuhanyu on 2021/1/7.
//

#ifndef NACOS_SDK_CPP_HOSTREACTOR_H
#define NACOS_SDK_CPP_HOSTREACTOR_H
#include <map>
#include "NacosString.h"
#include "naming/ServiceInfo.h"
#include "src/factory/ObjectConfigData.h"
#include "src/thread/RWLock.h"

namespace nacos {

class HostReactor {
private:
    ObjectConfigData *_objectConfigData;
    //memory cache
    std::map<NacosString, ServiceInfo> serviceInfoMap;
    //rw lock for serviceInfoMap
    RWLock rwLock;
public:
    HostReactor(ObjectConfigData *objectConfigData);
    void processServiceJson(const NacosString &json);
};

}

#endif //NACOS_SDK_CPP_HOSTREACTOR_H
