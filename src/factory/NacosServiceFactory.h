//
// Created by liuhanyu on 2020/8/30.
//

#ifndef NACOS_SDK_CPP_NACOSSERVICEFACTORY_H
#define NACOS_SDK_CPP_NACOSSERVICEFACTORY_H

#include "factory/INacosServiceFactory.h"
#include "Compatibility.h"
#include "src/thread/Mutex.h"

namespace nacos{

class AppConfigManager;
class ObjectConfigData;

class NacosServiceFactory : public INacosServiceFactory {
private:
    NacosString configFile;
    Properties props;
    bool configIsSet;
    bool propsIsSet;

    Mutex logSysInitLock;
    static volatile bool logSystemInitialized;
    void initializeRuntimeLogSettings(AppConfigManager *_appConfigManager);

    void checkConfig() NACOS_THROW(InvalidFactoryConfigException);
    AppConfigManager *buildConfigManager(ObjectConfigData *objectConfigData);

public:
    void setConfig(const NacosString &_configFile);

    void setProps(Properties &_props);

    virtual NamingService *CreateNamingService() NACOS_THROW(NacosException);

    virtual ConfigService *CreateConfigService() NACOS_THROW(NacosException);

    virtual NamingMaintainService *CreateNamingMaintainService() NACOS_THROW(NacosException);

    NacosServiceFactory();

    NacosServiceFactory(const NacosString &_configFile);

    NacosServiceFactory(Properties &_props);

    virtual ~NacosServiceFactory();
};

}//namespace nacos

#endif //NACOS_SDK_CPP_NACOSSERVICEFACTORY_H
