//
// Created by liuhanyu on 2020/8/30.
//

#ifndef NACOS_SDK_CPP_INACOSSERVICEFACTORY_H
#define NACOS_SDK_CPP_INACOSSERVICEFACTORY_H

#include "naming/NamingService.h"
#include "config/ConfigService.h"
#include "Properties.h"

class INacosServiceFactory {
public:
    virtual void setConfig(const NacosString &_configFile) = 0;

    virtual void setProps(Properties &_props) = 0;

    virtual NamingService *CreateNamingService() = 0;

    virtual ConfigService *CreateConfigService() = 0;

    ~INacosServiceFactory() {};
};


#endif //NACOS_SDK_CPP_INACOSSERVICEFACTORY_H
