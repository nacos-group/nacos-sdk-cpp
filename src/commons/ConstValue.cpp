#include "ConstValue.h"

namespace Nacos
{

const std::wstring ConstValue::ClientName = "NacosClient";
const std::wstring ConstValue::ClientVersion = "Nacos-CSharp-Client-v0.2.7";
const std::wstring ConstValue::RequestModule = "Naming";
const std::wstring ConstValue::DefaultGroup = "DEFAULT_GROUP";
std::wstring ConstValue::ENV_LIST_KEY = "envList";
std::wstring ConstValue::ALL_IPS = "000--00-ALL_IPS--00--000";
std::wstring ConstValue::FAILOVER_SWITCH = "00-00---000-VIPSRV_FAILOVER_SWITCH-000---00-00";
std::wstring ConstValue::DEFAULT_NAMESPACE_ID = "public";
int ConstValue::REQUEST_DOMAIN_RETRY_COUNT = 3;
std::wstring ConstValue::SERVER_ADDR_IP_SPLITER = ":";
std::wstring ConstValue::HTTP = "http://";
std::wstring ConstValue::HTTPS = "https://";
std::wstring ConstValue::HTTP_PREFIX = "http";
std::wstring ConstValue::ACCESS_TOKEN = "accessToken";
std::wstring ConstValue::TOKEN_TTL = "tokenTtl";
}
