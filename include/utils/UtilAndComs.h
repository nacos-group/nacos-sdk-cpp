#ifndef __UTIL_N_COMS_H_
#define __UTIL_N_COMS_H_
#include "NacosString.h"

class UtilAndComs
{
public:
	static NacosString VERSION;

	static NacosString WEB_CONTEXT;

	static NacosString NACOS_URL_BASE;

	static NacosString NACOS_URL_INSTANCE;

	static NacosString ENCODING;

	static NacosString ENV_LIST_KEY;

	static NacosString ALL_IPS;

	static NacosString FAILOVER_SWITCH;

	static NacosString DEFAULT_NAMESPACE_ID;

	static int REQUEST_DOMAIN_RETRY_COUNT;

	static NacosString NACOS_NAMING_LOG_NAME;

	static NacosString NACOS_NAMING_LOG_LEVEL;

	static NacosString SERVER_ADDR_IP_SPLITER;

	static int DEFAULT_CLIENT_BEAT_THREAD_COUNT;//TODO:Calc this according to nr_processors of the host

	static int DEFAULT_POLLING_THREAD_COUNT;//TODO:Calc this according to nr_processors of the host

	static void Init();
};

#endif
