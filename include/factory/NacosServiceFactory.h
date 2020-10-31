//
// Created by liuhanyu on 2020/8/30.
//

#ifndef NACOS_SDK_CPP_NACOSSERVICEFACTORY_H
#define NACOS_SDK_CPP_NACOSSERVICEFACTORY_H

#include "INacosServiceFactory.h"
namespace nacos{
class NacosServiceFactory : public INacosServiceFactory {
private:
    NacosString configFile;
    Properties props;
    bool configIsSet;
    bool propsIsSet;

    void checkConfig() throw(InvalidFactoryConfigException);

public:
    void setConfig(const NacosString &_configFile);

    void setProps(Properties &_props);

    virtual NamingService *CreateNamingService() throw(NacosException);

    virtual ConfigService *CreateConfigService() throw(NacosException);

    virtual NamingMaintainService *CreateNamingMaintainService() throw(NacosException);

    NacosServiceFactory();

    NacosServiceFactory(const NacosString &_configFile);

    NacosServiceFactory(Properties &_props);

    virtual ~NacosServiceFactory();
};

}//namespace nacos

#endif //NACOS_SDK_CPP_NACOSSERVICEFACTORY_H
