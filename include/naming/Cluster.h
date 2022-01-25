#ifndef __CLUSTER_H_
#define __CLUSTER_H_

#include <map>
#include "NacosString.h"

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
    NacosString getName() const;

    void setName(const NacosString &name);

    HealthChecker getHealthChecker() const;

    void setHealthChecker(const HealthChecker &healthChecker);

    std::map<NacosString, NacosString> getMetadata() const;

    void setMetadata(const std::map<NacosString, NacosString> &metadata);
};
}//namespace nacos

#endif