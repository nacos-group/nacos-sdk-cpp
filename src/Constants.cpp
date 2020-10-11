#include "Constants.h"

/**
 * Constant
 *
 * @author Nacos
 */

namespace nacos{
const NacosString Constants::CLIENT_VERSION = "3.0.0";

const int Constants::DATA_IN_BODY_VERSION = 204;

const NacosString Constants::DEFAULT_GROUP = "DEFAULT_GROUP";

const NacosString Constants::APPNAME = "AppName";

const NacosString Constants::UNKNOWN_APP = "UnknownApp";

const NacosString Constants::DEFAULT_DOMAINNAME = "commonconfig.config-host.taobao.com";

const NacosString Constants::DAILY_DOMAINNAME = "commonconfig.taobao.net";

const NacosString Constants::NUL = "";

const NacosString Constants::DATAID = "dataId";

const NacosString Constants::GROUP = "group";

const NacosString Constants::LAST_MODIFIED = "Last-Modified";

const NacosString Constants::ACCEPT_ENCODING = "Accept-Encoding";

const NacosString Constants::CONTENT_ENCODING = "Content-Encoding";

const NacosString Constants::PROBE_MODIFY_REQUEST = "Listening-Configs";

const NacosString Constants::PROBE_MODIFY_RESPONSE = "Probe-Modify-Response";

const NacosString Constants::PROBE_MODIFY_RESPONSE_NEW = "Probe-Modify-Response-New";

const NacosString Constants::USE_ZIP = "true";

const NacosString Constants::CONTENT_MD5 = "Content-MD5";

const NacosString Constants::CONFIG_VERSION = "Config-Version";

const NacosString Constants::IF_MODIFIED_SINCE = "If-Modified-Since";

const NacosString Constants::SPACING_INTERVAL = "client-spacing-interval";

const NacosString Constants::BASE_PATH = "/v1/cs";

const NacosString Constants::CONFIG_CONTROLLER_PATH = BASE_PATH + "/configs";

/**
* second
*/
const int Constants::ASYNC_UPDATE_ADDRESS_INTERVAL = 300;

/**
* second
*/
const int Constants::POLLING_INTERVAL_TIME = 15;

/**
* millisecond
*/
const int Constants::ONCE_TIMEOUT = 2000;

/**
* millisecond
*/
const int Constants::SO_TIMEOUT = 60000;

/**
* millisecond
*/
const int Constants::RECV_WAIT_TIMEOUT = ONCE_TIMEOUT * 5;

const NacosString Constants::ENCODE = "UTF-8";

const NacosString Constants::MAP_FILE = "map-file.js";

const int Constants::FLOW_CONTROL_THRESHOLD = 20;

const int Constants::FLOW_CONTROL_SLOT = 10;

const int Constants::FLOW_CONTROL_INTERVAL = 1000;

const NacosString Constants::LINE_SEPARATOR = "\x1";

const NacosString Constants::WORD_SEPARATOR = "\x2";

const NacosString Constants::LONGPOLLING_LINE_SEPARATOR = "\r\n";

const NacosString Constants::CLIENT_APPNAME_HEADER = "Client-AppName";
const NacosString Constants::CLIENT_REQUEST_TS_HEADER = "Client-RequestTS";
const NacosString Constants::CLIENT_REQUEST_TOKEN_HEADER = "Client-RequestToken";

const int Constants::ATOMIC_MAX_SIZE = 1000;

const NacosString Constants::NAMING_INSTANCE_ID_SPLITTER = "#";
const int Constants::NAMING_INSTANCE_ID_SEG_COUNT = 4;
const NacosString Constants::NAMING_HTTP_HEADER_SPILIER = "\\|";

const NacosString Constants::DEFAULT_CLUSTER_NAME = "DEFAULT";

const NacosString Constants::DEFAULT_NAMESPACE_ID = "public";

const int Constants::WRITE_REDIRECT_CODE = 307;

const NacosString Constants::SERVICE_INFO_SPLITER = "@@";

const NacosString Constants::NULL_STRING = "null";

const NacosString Constants::FILE_SEPARATOR = "/";
}//namespace nacos
