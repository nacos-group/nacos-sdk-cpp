#include "utils/ParamUtils.h"
#include "AppConfigManager.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"
#include "IOUtils.h"
#include "NacosExceptions.h"
#include "Parameters.h"
#include "utils/DirUtils.h"
#include <vector>

using namespace std;

namespace nacos{
NacosString AppConfigManager::LINE_SEPARATOR = "\n";
NacosString AppConfigManager::KV_SEPARATOR = "=";

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

NacosString AppConfigManager::get(const NacosString &key) const {
    if (appConfig.count(key) == 0) {
        return NULLSTR;
    }

    Properties copyProps = appConfig;
    if (key.compare(PropertyKeyConst::NAMESPACE) == 0
        && copyProps[PropertyKeyConst::NAMESPACE].compare("Public") == 0)
    {
        return NULLSTR;
    }
    return copyProps[key];
}

void AppConfigManager::set(const NacosString &key, const NacosString &value) {
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
    appConfig[PropertyKeyConst::HTTP_REQ_TIMEOUT] = "50000";
    appConfig[PropertyKeyConst::SRVLISTMGR_REFRESH_INTERVAL] = "30000";
    appConfig[PropertyKeyConst::SRVLISTMGR_READ_TIMEOUT] = "3000";
    appConfig[PropertyKeyConst::CONTEXT_PATH] = DEFAULT_CONTEXT_PATH;
    appConfig[PropertyKeyConst::TCP_NAMING_POLL_INTERVAL] = "30000";//30 secs by default
    appConfig[PropertyKeyConst::CONFIG_LONGPULLLING_TIMEOUT] = "30000";//ms
    appConfig[PropertyKeyConst::CONFIG_GET_TIMEOUT] = "3000";//ms
    appConfig[PropertyKeyConst::HB_FAIL_WAIT_TIME] = "20000";//ms

    NacosString homedir = DirUtils::getHome();

    appConfig[PropertyKeyConst::NACOS_LOG_PATH] = homedir + Constants::FILE_SEPARATOR + "nacos" + Constants::FILE_SEPARATOR + "log";
    appConfig[PropertyKeyConst::NACOS_SNAPSHOT_PATH] = homedir + Constants::FILE_SEPARATOR + "nacos" + Constants::FILE_SEPARATOR + "snapshot";
    log_info("DEFAULT_LOG_PATH:%s\n", appConfig[PropertyKeyConst::NACOS_LOG_PATH].c_str());
    log_info("DEFAULT_SNAPSHOT_PATH:%s\n", appConfig[PropertyKeyConst::NACOS_SNAPSHOT_PATH].c_str());
}


void AppConfigManager::applyConfig(Properties &rhs) {
    for (map<NacosString, NacosString>::iterator it = rhs.begin();
         it != rhs.end(); it++) {
        appConfig[it->first] = it->second;
    }
}
}//namespace nacos