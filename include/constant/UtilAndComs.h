#ifndef __UTIL_N_COMS_H_
#define __UTIL_N_COMS_H_

#include "NacosString.h"

namespace nacos{
class UtilAndComs {
public:
    static NacosString VERSION;

    static NacosString ENCODING;

    static NacosString NACOS_URL_BASE;
    
    static NacosString NACOS_URL_INSTANCE;

    static int REQUEST_DOMAIN_RETRY_COUNT;

    static NacosString SERVER_ADDR_IP_SPLITER;

    static int DEFAULT_CLIENT_BEAT_THREAD_COUNT;//TODO:Calc this according to nr_processors of the host

    static int DEFAULT_POLLING_THREAD_COUNT;//TODO:Calc this according to nr_processors of the host

    static void Init();
};
}//namespace nacos

#endif
