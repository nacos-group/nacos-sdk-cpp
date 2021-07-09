//
// Created by liuhanyu on 2021/7/6.
//

#include "ConfigProxy.h"
#include "src/http/HttpDelegate.h"
#include "utils/ParamUtils.h"

namespace nacos {

NacosString ConfigProxy::getDataToSign(const std::list <NacosString> &paramValues) {
    const NacosString & group = ParamUtils::findByKey(paramValues, "group");
    const NacosString & tenant = ParamUtils::findByKey(paramValues, "tenant");
    NacosString dataToSign = "";
    if (!NacosStringOps::isNullStr(tenant)) {
        dataToSign = tenant + "+";
    }
    if (!NacosStringOps::isNullStr(group)) {
        dataToSign += group;
    }

    return dataToSign;
}

HttpResult ConfigProxy::reqAPI
        (
                int method,
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    HttpDelegate *_httpDelegate = _objectConfigData->_httpDelegate;
    NacosString dataToSign = getDataToSign(paramValues);
    switch (method) {
        case IHttpCli::GET:
            return _httpDelegate->httpGet(path, headers, paramValues, encoding, readTimeoutMs);
        case IHttpCli::POST:
            return _httpDelegate->httpPost(path, headers, paramValues, encoding, readTimeoutMs);
        case IHttpCli::PUT:
            return _httpDelegate->httpPut(path, headers, paramValues, encoding, readTimeoutMs);
        case IHttpCli::DELETE:
            return _httpDelegate->httpDelete(path, headers, paramValues, encoding, readTimeoutMs);
        default://should never happen
            abort();
    }
}
}