#include "utils/ParamUtils.h"
#include "utils/DirUtils.h"
#include "config/AppConfigManager.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"
#include "config/IOUtils.h"
#include "NacosExceptions.h"
#include <vector>

using namespace std;

NacosString AppConfigManager::LINE_SEPARATOR = "\n";
NacosString AppConfigManager::KV_SEPARATOR = "=";
Properties AppConfigManager::parseConfigFile(const NacosString&file)
{
    Properties parsedConfig;
    NacosString confContent = IOUtils::readStringFromFile(file, NULLSTR);//TODO: add encoding support

    vector<NacosString> configList;
    ParamUtils::Explode(configList, confContent, LINE_SEPARATOR);

    for (vector<NacosString>::iterator it = configList.begin();
        it != configList.end(); it++)
    {
        if (ParamUtils::isBlank(ParamUtils::trim(*it)))
        {
            continue;
        }
        if (it->find(KV_SEPARATOR) == std::string::npos ||
            it->at(0) == '=')
        {
            throw MalformedConfigException(file);
        }
        vector<NacosString> configKV;
        ParamUtils::Explode(configKV, *it, KV_SEPARATOR);
        //k = v
        if (configKV.size() == 1)
        {
            parsedConfig[configKV[0]] = NULLSTR;
        }
        else
        {
            parsedConfig[configKV[0]] = configKV[1];
        }
    }

    return parsedConfig;
}


void AppConfigManager::checkReloadable() throw(NacosException)
{
    if (!reloadable)
    {
        throw NacosException(0, "This object is initialized as a non-reloadable one");
    }
}

size_t AppConfigManager::loadConfig() throw(NacosException)
{
    checkReloadable();
    appConfig = parseConfigFile(configFile);
    log_debug("loaded config file:%s\n", appConfig.toString().c_str());
    return appConfig.size();
}

size_t AppConfigManager::loadConfig(const NacosString &_configFile) throw(NacosException)
{
    checkReloadable();
    configFile = _configFile;
    loadConfig();
    return appConfig.size();
}

void AppConfigManager::clearConfig()
{
    appConfig.clear();
}

NacosString AppConfigManager::get(const NacosString&key) const
{
    if (appConfig.count(key) == 0)
    {
        return NULLSTR;
    }
    Properties copyProps = appConfig;
    return copyProps[key];
}

void AppConfigManager::set(const NacosString&key, const NacosString&value)
{
    appConfig[key] = value;
}

bool AppConfigManager::contains(const std::string &key) const
{
    return appConfig.contains(key);
}

AppConfigManager::AppConfigManager(Properties &props)
{
    reloadable = false;
    appConfig = props;
}

AppConfigManager::AppConfigManager(const NacosString &_configFile)
{
    reloadable = true;
    configFile = _configFile;
}