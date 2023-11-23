#include "src/utils/ParamUtils.h"
#include "AppConfigManager.h"
#include "NacosString.h"
#include "Properties.h"
#include "src/utils/NetUtils.h"
#include "constant/PropertyKeyConst.h"
#include "NacosExceptions.h"
#include "src/utils/DirUtils.h"
#include <vector>
#include <stdlib.h>
#include "src/log/Logger.h"
#include "src/utils/ConfigParserUtils.h"
#include "src/utils/Env.h"

using namespace std;

namespace nacos{

bool AppConfigManager::nacosAuthEnabled() {
    if (contains(PropertyKeyConst::AUTH_USERNAME) && contains(PropertyKeyConst::AUTH_PASSWORD)) {
        return true;
    } else {
        return false;
    }
}

void AppConfigManager::checkReloadable() NACOS_THROW(NacosException) {
    if (!reloadable) {
        throw NacosException(0, "This object is initialized as a non-reloadable one");
    }
}

size_t AppConfigManager::loadConfig() NACOS_THROW(NacosException) {
    checkReloadable();
    initDefaults();
    Properties parsedConfig = ConfigParserUtils::parseConfigFile(configFile);
    applyConfig(parsedConfig);
    log_debug("[AppConfigManager]-loadConfig:loaded config file:%s\n", appConfig.toString().c_str());
    return appConfig.size();
}

size_t AppConfigManager::loadConfig(const NacosString &_configFile) NACOS_THROW(NacosException) {
    checkReloadable();
    configFile = _configFile;
    loadConfig();
    return appConfig.size();
}

void AppConfigManager::clearConfig() {
    appConfig.clear();
}

const NacosString& AppConfigManager::get(const NacosString &key) {
    if (appConfig.count(key) == 0) {
        return NULLSTR;
    }

    if (key.compare(PropertyKeyConst::NAMESPACE) == 0
        && appConfig[PropertyKeyConst::NAMESPACE].compare("Public") == 0)
    {
        return NULLSTR;
    }

    Properties::iterator iter = appConfig.find(key);
    if (iter == appConfig.end()) {
        return NULLSTR;
    }
    
    return iter->second;
}

void AppConfigManager::set(const NacosString &key, const NacosString &value) {
    //Special case handle
    if (key.compare(PropertyKeyConst::SERVER_REQ_TIMEOUT) == 0) {
        _serverReqTimeout = atoi(value.c_str());
    } else if (key.compare(PropertyKeyConst::CONTEXT_PATH) == 0) {
        _contextPath = value;
    }
    appConfig[key] = value;
}

bool AppConfigManager::contains(const std::string &key) const {
    return appConfig.contains(key);
}

AppConfigManager::AppConfigManager(Properties &props) {
    reloadable = false;
    initDefaults();
    applyConfig(props);
}

AppConfigManager::AppConfigManager(const NacosString &_configFile) {
    reloadable = true;
    configFile = _configFile;
}

NacosString getAppNameFromEnv() {
    const char* env = getEnv("APP_NAME");
    if (env != NULL && std::char_traits<char>::length(env) > 0) {
        return NacosString(env);
    }

    return NacosStringOps::nullstr;
}

void AppConfigManager::initDefaults() {
    appConfig.clear();
    //appConfig[PropertyKeyConst::NAMESPACE] = "public";
    set(PropertyKeyConst::SRVLISTMGR_REFRESH_INTERVAL, "30000");
    set(PropertyKeyConst::SERVER_REQ_TIMEOUT, "3000");
    set(PropertyKeyConst::CONTEXT_PATH, ConfigConstant::DEFAULT_CONTEXT_PATH);
    set(PropertyKeyConst::SUBSCRIPTION_POLL_INTERVAL, "10000");//10 secs by default
    set(PropertyKeyConst::CONFIG_LONGPULLLING_TIMEOUT, "30000");//ms
    set(PropertyKeyConst::HB_FAIL_WAIT_TIME, "20000");//ms
    set(PropertyKeyConst::CLIENT_NAME, "default");
    set(PropertyKeyConst::LOCAL_IP, NetUtils::getHostIp());
    set(PropertyKeyConst::UDP_RECEIVER_PORT, "30620");

    NacosString appName = getAppNameFromEnv();
    if (!NacosStringOps::isNullStr(appName)) {
        set(PropertyKeyConst::APP_NAME, appName);
    }
    
    NacosString homedir = DirUtils::getHome();

    set(PropertyKeyConst::INSTANCE_ID_PREFIX, NetUtils::getHostName());
    set(PropertyKeyConst::INSTANCE_ID_SEQ_FILE, homedir + ConfigConstant::FILE_SEPARATOR + "nacos" + ConfigConstant::FILE_SEPARATOR + "instance_seq.dat");
    set(PropertyKeyConst::NACOS_SNAPSHOT_PATH, homedir + ConfigConstant::FILE_SEPARATOR + "nacos" + ConfigConstant::FILE_SEPARATOR + "snapshot");
    log_info("[AppConfigManager]-initDefaults:DEFAULT_SNAPSHOT_PATH:%s\n", appConfig[PropertyKeyConst::NACOS_SNAPSHOT_PATH].c_str());
}

void AppConfigManager::applyConfig(Properties &rhs) {
    for (map<NacosString, NacosString>::iterator it = rhs.begin();
         it != rhs.end(); it++) {
        set(it->first, it->second);
    }
}

}//namespace nacos