#include "naming/Instance.h"
#include "utils/ParamUtils.h"
#include "Debug.h"

Instance::Instance()
{
	weight = 1.0D;
	healthy = true;
	enabled = true;
	ephemeral = true;
}

NacosString Instance::toString()
{
	return "instanceId:" + instanceId + " ip:" + ip + " port:" + NacosStringOps::valueOf(port) +
	" weight:" + NacosStringOps::valueOf(weight) + " healthy:" + NacosStringOps::valueOf(healthy) +
	" enabled:" + NacosStringOps::valueOf(enabled) + " ephemeral:" + NacosStringOps::valueOf(ephemeral) +
	" clusterName:" + clusterName + " serviceName:" + serviceName + " metadata:{" + ParamUtils::Implode(metadata) + "}";
}
