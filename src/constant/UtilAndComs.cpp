#include "constant/UtilAndComs.h"

namespace nacos{
NacosString UtilAndComs::VERSION = "Nacos-C-Client:v1.0.21";//TODO:fix nacos trunk code for cpp client

NacosString UtilAndComs::ENCODING = "UTF-8";

NacosString UtilAndComs::NACOS_URL_BASE = "/v1/ns";

NacosString UtilAndComs::NACOS_URL_INSTANCE = NACOS_URL_BASE + "/instance";

int UtilAndComs::REQUEST_DOMAIN_RETRY_COUNT = 3;

NacosString UtilAndComs::SERVER_ADDR_IP_SPLITER = ":";

int UtilAndComs::DEFAULT_CLIENT_BEAT_THREAD_COUNT = 4;//TODO:Calc this according to nr_processors of the host

int UtilAndComs::DEFAULT_POLLING_THREAD_COUNT = 1;//TODO:Calc this according to nr_processors of the host

//Underlying logic:
/*int UtilAndComs::DEFAULT_CLIENT_BEAT_THREAD_COUNT = Runtime.getRuntime()
.availableProcessors() > 1 ? Runtime.getRuntime().availableProcessors() / 2
: 1;

int UtilAndComs::DEFAULT_POLLING_THREAD_COUNT = Runtime.getRuntime()
.availableProcessors() > 1 ? Runtime.getRuntime().availableProcessors() / 2
: 1;*/

void UtilAndComs::Init() {

}

}//namespace nacos
