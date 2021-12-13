//
// Created by liuhanyu on 2021/7/6.
//

#ifndef NACOS_SDK_CPP_CONFIGPROXY_H
#define NACOS_SDK_CPP_CONFIGPROXY_H
#include "src/factory/ObjectConfigData.h"
#include "src/http/IHttpCli.h"

namespace nacos {

class ConfigProxy {
private:
    ObjectConfigData *_objectConfigData;
    NacosString getDataToSign(const std::list <NacosString> &paramValues, const NacosString &nowTimeMs);
public:
    ConfigProxy(ObjectConfigData *objectConfigData) : _objectConfigData(objectConfigData) {};
    HttpResult reqAPI(int method, const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
                         const NacosString &encoding, long readTimeoutMs) NACOS_THROW(NetworkException);

};

}

#endif //NACOS_SDK_CPP_CONFIGPROXY_H
