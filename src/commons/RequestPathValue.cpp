#include "RequestPathValue.h"

namespace Nacos

{

const std::wstring RequestPathValue::CONFIGS = "/nacos/v1/cs/configs";
const std::wstring RequestPathValue::CONFIGS_LISTENER = "/nacos/v1/cs/configs/listener";
const std::wstring RequestPathValue::INSTANCE = "/nacos/v1/ns/instance";
const std::wstring RequestPathValue::INSTANCE_LIST = "/nacos/v1/ns/instance/list";
const std::wstring RequestPathValue::INSTANCE_BEAT = "/nacos/v1/ns/instance/beat";
const std::wstring RequestPathValue::INSTANCE_HEALTH = "/nacos/v1/ns/health/instance";
const std::wstring RequestPathValue::SERVICE = "/nacos/v1/ns/service";
const std::wstring RequestPathValue::SERVICE_LIST = "/nacos/v1/ns/service/list";
const std::wstring RequestPathValue::SWITCHES = "/nacos/v1/ns/operator/switches";
const std::wstring RequestPathValue::SERVERS = "/nacos/v1/ns/operator/servers";
const std::wstring RequestPathValue::LEADER = "/nacos/v1/ns/raft/leader";
const std::wstring RequestPathValue::METRICS = "/nacos/v1/ns/operator/metrics";
}
