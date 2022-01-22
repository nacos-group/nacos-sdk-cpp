#include "naming/Cluster.h"

namespace nacos{

NacosString Cluster::getName() const {
    return name;
}

void Cluster::setName(const NacosString &name) {
    Cluster::name = name;
}

HealthChecker Cluster::getHealthChecker() const {
    return healthChecker;
}

void Cluster::setHealthChecker(const HealthChecker &healthChecker) {
    Cluster::healthChecker = healthChecker;
}

std::map<NacosString, NacosString> Cluster::getMetadata() const {
    return metadata;
}

void Cluster::setMetadata(const std::map<NacosString, NacosString> &metadata) {
    Cluster::metadata = metadata;
}

}//namespace nacos
