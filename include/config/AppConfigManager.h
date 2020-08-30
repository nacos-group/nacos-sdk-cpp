#ifndef __APP_CFG_MGR_H_
#define __APP_CFG_MGR_H_
#include "NacosExceptions.h"
#include "NacosString.h"
#include "Properties.h"

class AppConfigManager
{
private:
    static Properties appConfig;
    static Properties parseConfigFile(const NacosString&file);
    static NacosString LINE_SEPARATOR;
    static NacosString KV_SEPARATOR;
public:
    static int loadConfig();
    static int clearConfig();
    static NacosString get(const NacosString&key);
    static void set(const NacosString&key, const NacosString&value);
};

#endif