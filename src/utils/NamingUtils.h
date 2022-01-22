#ifndef __NAMING_UTILS_H_
#define __NAMING_UTILS_H_

#include <vector>
#include "NacosString.h"
#include "constant/ConfigConstant.h"
#include "src/utils/ParamUtils.h"

namespace nacos{
class NamingUtils {
public:
    static NacosString getGroupedName(const NacosString &serviceName, const NacosString &groupName) {
        return groupName + ConfigConstant::SERVICE_INFO_SPLITER + serviceName;
    }

    static NacosString getServiceName(const NacosString &serviceNameWithGroup) {
        if (!ParamUtils::contains(serviceNameWithGroup, ConfigConstant::SERVICE_INFO_SPLITER)) {
            return serviceNameWithGroup;
        }
        std::vector <NacosString> splittedNameNGroup;
        ParamUtils::Explode(splittedNameNGroup, serviceNameWithGroup, ConfigConstant::SERVICE_INFO_SPLITER);
        return splittedNameNGroup[1];
    }

    static NacosString getGroupName(const NacosString &serviceNameWithGroup) {
        if (!ParamUtils::contains(serviceNameWithGroup, ConfigConstant::SERVICE_INFO_SPLITER)) {
            return ConfigConstant::DEFAULT_GROUP;
        }
        std::vector <NacosString> splittedNameNGroup;
        ParamUtils::Explode(splittedNameNGroup, serviceNameWithGroup, ConfigConstant::SERVICE_INFO_SPLITER);
        return splittedNameNGroup[0];
    }
};
}//namespace nacos

#endif
