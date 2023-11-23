#ifndef __APP_CFG_MGR_H_
#define __APP_CFG_MGR_H_

#include "NacosExceptions.h"
#include "NacosString.h"
#include "Properties.h"
#include "Compatibility.h"

namespace nacos{

class AppConfigManager {
private:
    bool reloadable;
    Properties appConfig;
    NacosString configFile;
    //Cached contextpath
    NacosString _contextPath;

    volatile long _serverReqTimeout;

    AppConfigManager();

    void checkReloadable() NACOS_THROW(NacosException);

    void initDefaults();

    void applyConfig(Properties &rhs);

public:
    bool nacosAuthEnabled();

    long getServeReqTimeout() const { return _serverReqTimeout; };

    bool isReloadable() const { return reloadable; };

    AppConfigManager(Properties &props);

    AppConfigManager(const NacosString &configFile);

    size_t loadConfig(const NacosString &configFile) NACOS_THROW(NacosException);

    size_t loadConfig() NACOS_THROW(NacosException);

    void clearConfig();

    const NacosString &get(const NacosString &key);

    bool contains(const NacosString &key) const;

    const Properties& getAllConfig() { return appConfig; };

    const NacosString & getContextPath() const { return _contextPath; };

    void set(const NacosString &key, const NacosString &value);
};
}//namespace nacos

#endif