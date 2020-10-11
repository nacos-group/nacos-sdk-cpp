#include "naming/Instance.h"
#include "utils/ParamUtils.h"
#include "Debug.h"

namespace nacos{
Instance::Instance() {
    weight = 1.0D;
    healthy = true;
    enabled = true;
    ephemeral = true;
    instanceId = "";
    ip = "";
    clusterName = "";
    serviceName = "";
    port = 0;
}

NacosString Instance::toString() const{
    return "instanceId:" + instanceId + " ip:" + ip + " port:" + NacosStringOps::valueOf(port) +
           " weight:" + NacosStringOps::valueOf(weight) + " healthy:" + NacosStringOps::valueOf(healthy) +
           " enabled:" + NacosStringOps::valueOf(enabled) + " ephemeral:" + NacosStringOps::valueOf(ephemeral) +
           " clusterName:" + clusterName + " serviceName:" + serviceName + " metadata:{" +
           ParamUtils::Implode(metadata) + "}";
}

NacosString Instance::toInetAddr() {
    return  ip + ":" + NacosStringOps::valueOf(port);
}

Instance & Instance::operator = (const Instance &rhs)
{
    this->instanceId = rhs.instanceId;
    this->ip = rhs.ip;
    this->port = rhs.port;
    this->weight = rhs.weight;
    this->healthy = rhs.healthy;
    this->enabled = rhs.enabled;
    this->ephemeral = rhs.ephemeral;
    this->clusterName = rhs.clusterName;
    this->metadata = rhs.metadata;
    return *this;
}

bool Instance::operator != (const Instance &rhs) const
{
    return !operator==(rhs);
}
bool Instance::operator == (const Instance &rhs) const
{
    return
    this->instanceId == rhs.instanceId &&
    this->ip == rhs.ip &&
    this->port == rhs.port &&
    this->weight == rhs.weight &&
    this->healthy == rhs.healthy &&
    this->enabled == rhs.enabled &&
    this->ephemeral == rhs.ephemeral &&
    this->clusterName == rhs.clusterName &&
    this->metadata == rhs.metadata;
}
}//namespace nacos