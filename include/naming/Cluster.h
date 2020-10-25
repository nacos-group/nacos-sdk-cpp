#ifndef __CLUSTER_H_
#define __CLUSTER_H_

#include <vector>
#include <list>
#include <sys/time.h>
#include "NacosString.h"
#include "Constants.h"
#include "utils/url.h"
#include "utils/ParamUtils.h"
#include "naming/Instance.h"

namespace nacos{
class HealthChecker {
private:
    NacosString type;
public:
    NacosString getType() const { return type; };
    void setType(const NacosString &_type) { type = _type; };
};
class Cluster {
private:
    NacosString name;
    HealthChecker healthChecker;
    std::map<NacosString, NacosString> metadata;
public:
    NacosString getName() const {
        return name;
    }

    void setName(const NacosString &name) {
        Cluster::name = name;
    }

    HealthChecker getHealthChecker() const {
        return healthChecker;
    }

    void setHealthChecker(const HealthChecker &healthChecker) {
        Cluster::healthChecker = healthChecker;
    }

    std::map<NacosString, NacosString> getMetadata() const {
        return metadata;
    }

    void setMetadata(const std::map<NacosString, NacosString> &metadata) {
        Cluster::metadata = metadata;
    }
};
}//namespace nacos

#endif