//
// Created by liuhanyu on 2021/1/7.
//

#include "HostReactor.h"
#include "src/json/JSON.h"
#include "utils/NamingUtils.h"
#include "src/naming/subscribe/EventDispatcher.h"

namespace nacos {

HostReactor::HostReactor(ObjectConfigData *objectConfigData) {
    _objectConfigData = objectConfigData;
}

void HostReactor::processServiceJson(const NacosString &json) {
    ServiceInfo serviceInfo = JSON::JsonStr2ServiceInfo(json);

    NacosString name = NamingUtils::getGroupedName(serviceInfo.getName(), serviceInfo.getGroupName());
    NacosString key = ServiceInfo::getKey(name, serviceInfo.getClusters());
    WriteGuard _writeGuard(rwLock);
    if (serviceInfoMap.count(key) == 0) {
        //The first time, don't notify
        serviceInfoMap[key] = serviceInfo;
    } else {
        ServiceInfo oldServiceInfo = serviceInfoMap[key];
        ChangeAdvice changeAdvice;
        changeAdvice.key = key;
        ChangeAdvice::compareChange(oldServiceInfo, serviceInfo, changeAdvice);
        log_debug("Change status:modified:%d added:%d removed:%d\n", changeAdvice.modified, changeAdvice.added, changeAdvice.removed);
        if (changeAdvice.modified || changeAdvice.added || changeAdvice.removed) {
            //asm volatile("int $3");
            changeAdvice.newServiceInfo = serviceInfo;
            _objectConfigData->_eventDispatcher->notifyDirectly(changeAdvice);
        }
        serviceInfoMap[key] = serviceInfo;//update local service info to the new one
    }
}


}