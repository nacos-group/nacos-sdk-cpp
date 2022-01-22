//
// Created by liuhanyu on 2021/1/9.
//

#include <vector>
#include "ConfigParserUtils.h"
#include "src/config/IOUtils.h"
#include "src/utils/ParamUtils.h"
#include "constant/ConfigConstant.h"
using namespace std;

namespace nacos {

Properties ConfigParserUtils::parseConfigFile(const NacosString &file) NACOS_THROW(NacosException) {
    Properties parsedConfig;
    NacosString confContent = IOUtils::readStringFromFile(file, NULLSTR);//TODO: add encoding support

    vector <NacosString> configList;
    ParamUtils::Explode(configList, confContent, ConfigConstant::CONFIG_NEXT_LINE);

    int line = 0;
    for (vector<NacosString>::iterator it = configList.begin();
         it != configList.end(); it++) {
        line++;
        NacosString trimmedLine = ParamUtils::trim(*it);
        if (ParamUtils::isBlank(trimmedLine)) {
            continue;
        }

        if (trimmedLine[0] == '#') {
            //skip comment
            continue;
        }

        if (it->find(ConfigConstant::CONFIG_KV_SEPARATOR) == std::string::npos) {
            throw MalformedConfigException(file, " no '=' found at line " + NacosStringOps::valueOf(line));
        }

        vector <NacosString> configKV;
        ParamUtils::Explode(configKV, *it, ConfigConstant::CONFIG_KV_SEPARATOR);
        //k = v
        NacosString key = ParamUtils::trim(configKV[0]);
        if (ParamUtils::isBlank(key)) {
            throw MalformedConfigException(file, " key is blank at " + NacosStringOps::valueOf(line));
        }

        if (configKV.size() == 1) {
            parsedConfig[key] = NULLSTR;
        } else {
            parsedConfig[key] = configKV[1];
        }
    }

    return parsedConfig;
}

}