#ifndef __CONSTANTS_H_
#define __CONSTANTS_H_

#include "NacosString.h"

/**
 * Constant
 *
 * @author Nacos
 */
class Constants {
public:
    const static NacosString CLIENT_VERSION;

    const static int DATA_IN_BODY_VERSION;

    const static NacosString DEFAULT_GROUP;

    const static NacosString APPNAME;

    const static NacosString UNKNOWN_APP;

    const static NacosString DEFAULT_DOMAINNAME;

    const static NacosString DAILY_DOMAINNAME;

    const static NacosString NUL;

    const static NacosString DATAID;

    const static NacosString GROUP;

    const static NacosString LAST_MODIFIED;

    const static NacosString ACCEPT_ENCODING;

    const static NacosString CONTENT_ENCODING;

    const static NacosString PROBE_MODIFY_REQUEST;

    const static NacosString PROBE_MODIFY_RESPONSE;

    const static NacosString PROBE_MODIFY_RESPONSE_NEW;

    const static NacosString USE_ZIP;

    const static NacosString CONTENT_MD5;

    const static NacosString CONFIG_VERSION;

    const static NacosString IF_MODIFIED_SINCE;

    const static NacosString SPACING_INTERVAL;

    const static NacosString BASE_PATH;

    const static NacosString CONFIG_CONTROLLER_PATH;

    /**
     * second
     */
    const static int ASYNC_UPDATE_ADDRESS_INTERVAL;

    /**
     * second
     */
    const static int POLLING_INTERVAL_TIME;

    /**
     * millisecond
     */
    const static int ONCE_TIMEOUT;

    /**
     * millisecond
     */
    const static int SO_TIMEOUT;

    /**
     * millisecond
     */
    const static int RECV_WAIT_TIMEOUT;

    const static NacosString ENCODE;

    const static NacosString MAP_FILE;

    const static int FLOW_CONTROL_THRESHOLD;

    const static int FLOW_CONTROL_SLOT;

    const static int FLOW_CONTROL_INTERVAL;

    const static NacosString LINE_SEPARATOR;

    const static NacosString WORD_SEPARATOR;

    const static NacosString LONGPOLLING_LINE_SEPARATOR;

    const static NacosString CLIENT_APPNAME_HEADER;
    const static NacosString CLIENT_REQUEST_TS_HEADER;
    const static NacosString CLIENT_REQUEST_TOKEN_HEADER;

    const static int ATOMIC_MAX_SIZE;

    const static NacosString NAMING_INSTANCE_ID_SPLITTER;
    const static int NAMING_INSTANCE_ID_SEG_COUNT;
    const static NacosString NAMING_HTTP_HEADER_SPILIER;

    const static NacosString DEFAULT_CLUSTER_NAME;

    const static NacosString DEFAULT_NAMESPACE_ID;

    const static int WRITE_REDIRECT_CODE;

    const static NacosString SERVICE_INFO_SPLITER;

    const static NacosString NULL_STRING;

    const static NacosString FILE_SEPARATOR;
};

#endif
