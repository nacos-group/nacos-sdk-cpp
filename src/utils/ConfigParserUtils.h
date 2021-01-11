//
// Created by liuhanyu on 2021/1/9.
//

#ifndef NACOS_SDK_CPP_CONFIGPARSERUTILS_H
#define NACOS_SDK_CPP_CONFIGPARSERUTILS_H
#include "Properties.h"
#include "NacosExceptions.h"

namespace nacos {

class ConfigParserUtils {
public:
    static Properties parseConfigFile(const NacosString &file) throw(NacosException);

};

}

#endif //NACOS_SDK_CPP_CONFIGPARSERUTILS_H
