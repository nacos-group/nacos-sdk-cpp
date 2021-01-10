//
// Created by liuhanyu on 2021/1/9.
//

#include <vector>
#include "ConfigParserUtils.h"
#include "src/config/IOUtils.h"
#include "utils/ParamUtils.h"
#include "constant/ConfigConstant.h"
using namespace std;

namespace nacos {

Properties ConfigParserUtils::parseConfigFile(const NacosString &file) throw(NacosException) {
    Properties parsedConfig;
    NacosString confContent = IOUtils::readStringFromFile(file, NULLSTR);//TODO: add encoding support

    vector <NacosString> configList;
    ParamUtils::Explode(configList, confContent, ConfigConstant::CONFIG_NEXT_LINE);

    for (vector<NacosString>::iterator it = configList.begin();
         it != configList.end(); it++) {
        if (ParamUtils::isBlank(ParamUtils::trim(*it))) {
            continue;
        }
        if (it->find(ConfigConstant::CONFIG_KV_SEPARATOR) == std::string::npos ||
            it->at(0) == '=') {
            throw MalformedConfigException(file);
        }
        vector <NacosString> configKV;
        ParamUtils::Explode(configKV, *it, ConfigConstant::CONFIG_KV_SEPARATOR);
        //k = v
        if (configKV.size() == 1) {
            parsedConfig[configKV[0]] = NULLSTR;
        } else {
            parsedConfig[configKV[0]] = configKV[1];
        }
    }

    return parsedConfig;
}

}