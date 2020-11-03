//
// Created by liuhanyu on 2020/8/30.
//

#ifndef NACOS_SDK_CPP_INACOSSERVICEFACTORY_H
#define NACOS_SDK_CPP_INACOSSERVICEFACTORY_H

#include "naming/NamingService.h"
#include "naming/NamingMaintainService.h"
#include "config/ConfigService.h"
#include "Properties.h"

namespace nacos{
class INacosServiceFactory {
public:
    virtual void setConfig(const NacosString &_configFile) = 0;

    virtual void setProps(Properties &_props) = 0;

    virtual NamingService *CreateNamingService() = 0;

    virtual ConfigService *CreateConfigService() = 0;

    virtual NamingMaintainService *CreateNamingMaintainService() = 0;

    ~INacosServiceFactory() {};
};

}//namespace nacos

#endif //NACOS_SDK_CPP_INACOSSERVICEFACTORY_H
