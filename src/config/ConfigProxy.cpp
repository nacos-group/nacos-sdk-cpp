//
// Created by liuhanyu on 2021/7/6.
//

#include "ConfigProxy.h"
#include "src/http/HttpDelegate.h"
#include "src/utils/ParamUtils.h"
#include "src/utils/TimeUtils.h"
#include "src/config/AppConfigManager.h"
#include "constant/PropertyKeyConst.h"
#include "src/crypto/SignatureTool.h"

namespace nacos {

NacosString ConfigProxy::getDataToSign(const std::list <NacosString> &paramValues, const NacosString &nowTimeMs) {
    const NacosString & group = ParamUtils::findByKey(paramValues, "group");
    const NacosString & tenant = ParamUtils::findByKey(paramValues, "tenant");
    NacosString dataToSign = "";
    if (!NacosStringOps::isNullStr(tenant)) {
        dataToSign = tenant + "+";
    }
    if (!NacosStringOps::isNullStr(group)) {
        dataToSign += group;
    }

    if (!NacosStringOps::isNullStr(dataToSign)) {
        dataToSign += "+" + nowTimeMs;
    } else {
        dataToSign = nowTimeMs;
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

    //TODO: refactor to a common procedure
    const NacosString& secretKey = _objectConfigData->_appConfigManager->get(PropertyKeyConst::SECRET_KEY);
    const NacosString& accessKey = _objectConfigData->_appConfigManager->get(PropertyKeyConst::ACCESS_KEY);
    const NacosString& appName = _objectConfigData->_appConfigManager->get(PropertyKeyConst::APP_NAME);

    //If SPAS security credentials are set, SPAS is enabled
    if (!ParamUtils::isBlank(secretKey) && !ParamUtils::isBlank(accessKey)) {
        NacosString nowTimeMs = NacosStringOps::valueOf(TimeUtils::getCurrentTimeInMs());
        NacosString dataToSign = getDataToSign(paramValues, nowTimeMs);
        NacosString signature = SignatureTool::SignWithHMAC_SHA1(dataToSign, secretKey);
        ParamUtils::addKV(headers, "Spas-Signature", signature);
        ParamUtils::addKV(headers, "Timestamp", nowTimeMs);
        ParamUtils::addKV(headers, "Spas-AccessKey", accessKey);
    }


    if (!NacosStringOps::isNullStr(appName)) {
        ParamUtils::addKV(headers, "Client-AppName", appName);
    }

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