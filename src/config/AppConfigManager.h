#ifndef __APP_CFG_MGR_H_
#define __APP_CFG_MGR_H_

#include "NacosExceptions.h"
#include "NacosString.h"
#include "Properties.h"

namespace nacos{
class AppConfigManager {
private:
    bool reloadable;
    Properties appConfig;
    NacosString configFile;

    static Properties parseConfigFile(const NacosString &file);

    static NacosString LINE_SEPARATOR;
    static NacosString KV_SEPARATOR;

    AppConfigManager();

    void checkReloadable() throw(NacosException);

    void initDefaults();

    void applyConfig(Properties &rhs);

public:
    bool isReloadable() const { return reloadable; };

    AppConfigManager(Properties &props);

    AppConfigManager(const NacosString &configFile);

    size_t loadConfig(const NacosString &configFile) throw(NacosException);

    size_t loadConfig() throw(NacosException);

    void clearConfig();

    NacosString get(const NacosString &key) const;

    bool contains(const NacosString &key) const;

    Properties getAllConfig() const { return appConfig; };

    void set(const NacosString &key, const NacosString &value);
};
}//namespace nacos

#endif