#ifndef __PROP_KEY_CONST_H_
#define __PROP_KEY_CONST_H_

#include "NacosString.h"

class PropertyKeyConst {
public:
    static const NacosString CONFIG_FILE_NAME;

    static const NacosString IS_USE_ENDPOINT_PARSING_RULE;

    static const NacosString ENDPOINT;

    static const NacosString ENDPOINT_PORT;

    static const NacosString NAMESPACE;

    static const NacosString ACCESS_KEY;

    static const NacosString SECRET_KEY;

    static const NacosString RAM_ROLE_NAME;

    static const NacosString SERVER_ADDR;

    static const NacosString CONTEXT_PATH;

    static const NacosString CLUSTER_NAME;

    static const NacosString ENCODE;

    static const NacosString NAMING_LOAD_CACHE_AT_START;

    static const NacosString NAMING_CLIENT_BEAT_THREAD_COUNT;

    static const NacosString NAMING_POLLING_THREAD_COUNT;

    static const NacosString HTTP_REQ_TIMEOUT;

    static const NacosString SRVLISTMGR_REFRESH_INTERVAL;

    static const NacosString SRVLISTMGR_READ_TIMEOUT;

    static const NacosString TCP_NAMING_POLL_INTERVAL;

    static const NacosString CONFIG_LONGPULLLING_TIMEOUT;

    static const NacosString CONFIG_GET_TIMEOUT;

    static const NacosString HB_FAIL_WAIT_TIME;

    /*public static class SystemEnv {

        static const NacosString ALIBABA_ALIWARE_ENDPOINT_PORT = "ALIBABA_ALIWARE_ENDPOINT_PORT";

        static const NacosString ALIBABA_ALIWARE_NAMESPACE = "ALIBABA_ALIWARE_NAMESPACE";

        static const NacosString ALIBABA_ALIWARE_ENDPOINT_URL = "ALIBABA_ALIWARE_ENDPOINT_URL";
    }*/
};

#endif