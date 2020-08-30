#include "utils/ParamUtils.h"
#include "utils/DirUtils.h"
#include "config/AppConfigManager.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"
#include "config/IOUtils.h"
#include <vector>

using namespace std;

Properties AppConfigManager::appConfig;
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
        vector<NacosString> configKV;
        ParamUtils::Explode(configKV, *it, KV_SEPARATOR);
        parsedConfig[configKV[0]] = configKV[1];//k = v
    }

    return parsedConfig;
}

int AppConfigManager::loadConfig()
{
    NacosString configFile = DirUtils::getCwd() + Constants::FILE_SEPARATOR + PropertyKeyConst::CONFIG_FILE_NAME;
    appConfig = parseConfigFile(configFile);
}
int AppConfigManager::clearConfig()
{
    appConfig.clear();
}
NacosString AppConfigManager::get(const NacosString&key)
{
    if (appConfig.count(key) == 0)
    {
        return NULLSTR;
    }
    return appConfig[key];
}
void AppConfigManager::set(const NacosString&key, const NacosString&value)
{
    appConfig[key] = value;
}