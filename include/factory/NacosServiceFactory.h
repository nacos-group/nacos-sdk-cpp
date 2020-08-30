//
// Created by liuhanyu on 2020/8/30.
//

#ifndef NACOS_SDK_CPP_NACOSSERVICEFACTORY_H
#define NACOS_SDK_CPP_NACOSSERVICEFACTORY_H
#include "naming/NamingService.h"
#include "config/ConfigService.h"

class NacosServiceFactory {
private:
public:
    NamingService *CreateNamingService() {//TODO
    };
    ConfigService *CreateConfigService() {//TODO
    };
};


#endif //NACOS_SDK_CPP_NACOSSERVICEFACTORY_H
