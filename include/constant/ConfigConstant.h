#ifndef __CONFIG_CONSTANTS_H_
#define __CONFIG_CONSTANTS_H_

#include "NacosString.h"

/**
 * Constant
 *
 * @author Nacos
 */

namespace nacos{

//Constants for config service
class ConfigConstant {
public:

    const static NacosString DEFAULT_GROUP;

    const static NacosString DEFAULT_CONTEXT_PATH;

    const static NacosString PROTOCOL_VERSION;

    const static NacosString GET_SERVERS_PATH;

    const static NacosString DATAID;

    const static NacosString PROBE_MODIFY_REQUEST;

    const static NacosString PROBE_MODIFY_RESPONSE;

    const static NacosString BASE_PATH;

    const static NacosString CONFIG_CONTROLLER_PATH;

    /**
     * second
     */
    const static int POLLING_INTERVAL_TIME;

    const static NacosString ENCODE;

    const static int FLOW_CONTROL_THRESHOLD;

    const static NacosString LINE_SEPARATOR;

    const static NacosString WORD_SEPARATOR;

    const static NacosString NAMING_INSTANCE_ID_SPLITTER;

    const static NacosString DEFAULT_CLUSTER_NAME;

    const static NacosString SERVICE_INFO_SPLITER;

    const static NacosString FILE_SEPARATOR;

    const static NacosString CONFIG_NEXT_LINE;

    const static NacosString CONFIG_KV_SEPARATOR;

    const static NacosString DEFAULT_CONFIG_FILE;
};
}//namespace nacos

#endif
