#include "utils/ParamUtils.h"
#include "AppConfigManager.h"
#include "NacosString.h"
#include "Properties.h"
#include "src/utils/NetUtils.h"
#include "constant/PropertyKeyConst.h"
#include "IOUtils.h"
#include "NacosExceptions.h"
#include "utils/DirUtils.h"
#include <vector>
#include <stdlib.h>

using namespace std;

namespace nacos{
NacosString AppConfigManager::LINE_SEPARATOR = "\n";
NacosString AppConfigManager::KV_SEPARATOR = "=";


bool AppConfigManager::nacosAuthEnabled() {
    if (contains(PropertyKeyConst::AUTH_USERNAME) && contains(PropertyKeyConst::AUTH_PASSWORD)) {
        return true;
    } else {
        return false;
    }
}

Properties AppConfigManager::parseConfigFile(const NacosString &file) {
    Properties parsedConfig;
    NacosString confContent = IOUtils::readStringFromFile(file, NULLSTR);//TODO: add encoding support

    vector <NacosString> configList;
    ParamUtils::Explode(configList, confContent, LINE_SEPARATOR);

    for (vector<NacosString>::iterator it = configList.begin();
         it != configList.end(); it++) {
        if (ParamUtils::isBlank(ParamUtils::trim(*it))) {
            continue;
        }
        if (it->find(KV_SEPARATOR) == std::string::npos ||
            it->at(0) == '=') {
            throw MalformedConfigException(file);
        }
        vector <NacosString> configKV;
        ParamUtils::Explode(configKV, *it, KV_SEPARATOR);
        //k = v
        if (configKV.size() == 1) {
            parsedConfig[configKV[0]] = NULLSTR;
        } else {
            parsedConfig[configKV[0]] = configKV[1];
        }
    }

    return parsedConfig;
}


void AppConfigManager::checkReloadable() throw(NacosException) {
    if (!reloadable) {
        throw NacosException(0, "This object is initialized as a non-reloadable one");
    }
}

size_t AppConfigManager::loadConfig() throw(NacosException) {
    checkReloadable();
    initDefaults();
    Properties parsedConfig = parseConfigFile(configFile);
    applyConfig(parsedConfig);
    log_debug("loaded config file:%s\n", appConfig.toString().c_str());
    return appConfig.size();
}

size_t AppConfigManager::loadConfig(const NacosString &_configFile) throw(NacosException) {
    checkReloadable();
    configFile = _configFile;
    loadConfig();
    return appConfig.size();
}

void AppConfigManager::clearConfig() {
    appConfig.clear();
}

const NacosString &AppConfigManager::get(const NacosString &key) {
    if (appConfig.count(key) == 0) {
        return NULLSTR;
    }

    if (key.compare(PropertyKeyConst::NAMESPACE) == 0
        && appConfig[PropertyKeyConst::NAMESPACE].compare("Public") == 0)
    {
        return NULLSTR;
    }
    return appConfig[key];
}

void AppConfigManager::set(const NacosString &key, const NacosString &value) {
    //Special case handle
    if (key.compare(PropertyKeyConst::SERVER_REQ_TIMEOUT) == 0) {
        _serverReqTimeout = atoi(value.c_str());
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

void AppConfigManager::initDefaults() {
    appConfig.clear();
    //appConfig[PropertyKeyConst::NAMESPACE] = "public";
    set(PropertyKeyConst::SRVLISTMGR_REFRESH_INTERVAL, "30000");
    set(PropertyKeyConst::SERVER_REQ_TIMEOUT, "3000");
    set(PropertyKeyConst::CONTEXT_PATH, ConfigConstant::DEFAULT_CONTEXT_PATH);
    set(PropertyKeyConst::TCP_NAMING_POLL_INTERVAL, "30000");//30 secs by default
    set(PropertyKeyConst::CONFIG_LONGPULLLING_TIMEOUT, "30000");//ms
    set(PropertyKeyConst::HB_FAIL_WAIT_TIME, "20000");//ms
    set(PropertyKeyConst::CLIENT_NAME, "default");
    set(PropertyKeyConst::LOCAL_IP, NetUtils::getHostIp());

    NacosString homedir = DirUtils::getHome();

    set(PropertyKeyConst::NACOS_LOG_PATH, homedir + ConfigConstant::FILE_SEPARATOR + "nacos" + ConfigConstant::FILE_SEPARATOR + "log");
    set(PropertyKeyConst::NACOS_SNAPSHOT_PATH, homedir + ConfigConstant::FILE_SEPARATOR + "nacos" + ConfigConstant::FILE_SEPARATOR + "snapshot");
    log_info("DEFAULT_LOG_PATH:%s\n", appConfig[PropertyKeyConst::NACOS_LOG_PATH].c_str());
    log_info("DEFAULT_SNAPSHOT_PATH:%s\n", appConfig[PropertyKeyConst::NACOS_SNAPSHOT_PATH].c_str());
}


void AppConfigManager::applyConfig(Properties &rhs) {
    for (map<NacosString, NacosString>::iterator it = rhs.begin();
         it != rhs.end(); it++) {
        set(it->first, it->second);
    }
}

}//namespace nacos