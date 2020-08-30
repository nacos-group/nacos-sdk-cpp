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

size_t AppConfigManager::loadConfig(const NacosString &configFile)
{
    appConfig = parseConfigFile(configFile);
    log_debug("loaded config file:%s\n", appConfig.toString().c_str());
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