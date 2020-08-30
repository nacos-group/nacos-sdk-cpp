#ifndef __APP_CFG_MGR_H_
#define __APP_CFG_MGR_H_
#include "NacosExceptions.h"
#include "NacosString.h"
#include "Properties.h"

class AppConfigManager
{
private:
    Properties appConfig;
    static Properties parseConfigFile(const NacosString&file);
    static NacosString LINE_SEPARATOR;
    static NacosString KV_SEPARATOR;
public:
    size_t loadConfig(const NacosString &configFile);
    void clearConfig();
    NacosString get(const NacosString&key) const;
    Properties getAllConfig() const { return appConfig; };
    void set(const NacosString&key, const NacosString&value);
};

#endif