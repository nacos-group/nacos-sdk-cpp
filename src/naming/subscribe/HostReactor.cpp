//
// Created by liuhanyu on 2021/1/7.
//

#include "HostReactor.h"
#include "src/json/JSON.h"
#include "src/utils/NamingUtils.h"
#include "src/naming/subscribe/EventDispatcher.h"

namespace nacos {

HostReactor::HostReactor(ObjectConfigData *objectConfigData) {
    _objectConfigData = objectConfigData;
}

void HostReactor::processServiceJson(const NacosString &json) {
    ServiceInfo serviceInfo = JSON::JsonStr2ServiceInfo(json);

    NacosString name = NamingUtils::getGroupedName(serviceInfo.getName(), serviceInfo.getGroupName());
    NacosString key = ServiceInfo::getKey(name, serviceInfo.getClusters());
    ServiceInfo oldServiceInfo;
    bool newServiceInfo = false;
    {
        WriteGuard _writeGuard(rwLock);
        if (serviceInfoMap.count(key) == 0) {
            if (serviceInfo.ipCount()==0) {
                log_warn("hosts got from server is empty.\n");
                return;
            }
            serviceInfoMap[key] = serviceInfo;
            newServiceInfo = true;
        } else {
            oldServiceInfo = serviceInfoMap[key];
            if (oldServiceInfo.getLastRefTime() >= serviceInfo.getLastRefTime()) {
                log_warn("ServiceInfo got from server is older than the one in client.\n");
                return;
            }
            serviceInfoMap[key] = serviceInfo;//update local service info to the new one
        }
    }
    ChangeAdvice changeAdvice;
    changeAdvice.key = key;
    if (newServiceInfo) {
        changeAdvice.added = true;
        changeAdvice.newServiceInfo = serviceInfo;
        _objectConfigData->_eventDispatcher->notifyDirectly(changeAdvice);
    } else {//service info is updated
        ChangeAdvice::compareChange(oldServiceInfo, serviceInfo, changeAdvice);
        log_debug("Change status:modified:%d added:%d removed:%d\n", changeAdvice.modified, changeAdvice.added,
                  changeAdvice.removed);
        if (changeAdvice.modified || changeAdvice.added || changeAdvice.removed) {
            //asm volatile("int $3");
            changeAdvice.newServiceInfo = serviceInfo;
            _objectConfigData->_eventDispatcher->notifyDirectly(changeAdvice);
        }
    }
}

}