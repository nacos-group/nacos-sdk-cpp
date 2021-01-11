#include "constant/ConfigConstant.h"

/**
 * Constant
 *
 * @author Nacos
 */

namespace nacos{
const NacosString ConfigConstant::DEFAULT_GROUP = "DEFAULT_GROUP";
const NacosString ConfigConstant::DEFAULT_CONTEXT_PATH = "nacos";
const NacosString ConfigConstant::PROTOCOL_VERSION = "v1";
const NacosString ConfigConstant::GET_SERVERS_PATH = "ns/operator/servers";

const NacosString ConfigConstant::DATAID = "dataId";

const NacosString ConfigConstant::PROBE_MODIFY_REQUEST = "Listening-Configs";

const NacosString ConfigConstant::PROBE_MODIFY_RESPONSE = "Probe-Modify-Response";

const NacosString ConfigConstant::BASE_PATH = "/v1/cs";

const NacosString ConfigConstant::CONFIG_CONTROLLER_PATH = BASE_PATH + "/configs";

/**
* second
*/
const int ConfigConstant::POLLING_INTERVAL_TIME = 15;

const NacosString ConfigConstant::ENCODE = "UTF-8";

const int ConfigConstant::FLOW_CONTROL_THRESHOLD = 20;

const NacosString ConfigConstant::LINE_SEPARATOR = "\x1";

const NacosString ConfigConstant::WORD_SEPARATOR = "\x2";

const NacosString ConfigConstant::NAMING_INSTANCE_ID_SPLITTER = "#";

const NacosString ConfigConstant::DEFAULT_CLUSTER_NAME = "DEFAULT";

const NacosString ConfigConstant::SERVICE_INFO_SPLITER = "@@";

const NacosString ConfigConstant::FILE_SEPARATOR = "/";

const NacosString ConfigConstant::CONFIG_NEXT_LINE = "\n";

const NacosString ConfigConstant::CONFIG_KV_SEPARATOR = "=";

const NacosString ConfigConstant::DEFAULT_CONFIG_FILE = "nacos-cpp-cli.properties";
}//namespace nacos
