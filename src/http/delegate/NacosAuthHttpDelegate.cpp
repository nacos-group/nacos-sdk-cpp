//
// Created by liuhanyu on 2020/12/5.
//

#include "NacosAuthHttpDelegate.h"
#include "src/security/SecurityManager.h"

using namespace std;

namespace nacos {
NacosString NacosAuthHttpDelegate::getEncode() const {
    return encoding;
}

NacosAuthHttpDelegate::NacosAuthHttpDelegate(ObjectConfigData *objectConfigData) {
    _objectConfigData = objectConfigData;
}

HttpResult NacosAuthHttpDelegate::httpGet
(
    const NacosString &path,
    std::list<NacosString> &headers,
    std::list<NacosString> &paramValues,
    const NacosString &_encoding,
    long readTimeoutMs
) NACOS_THROW(NetworkException) {
    HttpResult res;
    _objectConfigData->_securityManager->addAccessToken2Req(paramValues);
    res = _objectConfigData->_httpCli->httpGet(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NacosAuthHttpDelegate::httpDelete
(
    const NacosString &path,
    std::list<NacosString> &headers,
    std::list<NacosString> &paramValues,
    const NacosString &_encoding,
    long readTimeoutMs
) NACOS_THROW(NetworkException) {
    HttpResult res;
    _objectConfigData->_securityManager->addAccessToken2Req(paramValues);
    res = _objectConfigData->_httpCli->httpDelete(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NacosAuthHttpDelegate::httpPost
(
    const NacosString &path,
    std::list<NacosString> &headers,
    std::list<NacosString> &paramValues,
    const NacosString &_encoding,
    long readTimeoutMs
) NACOS_THROW(NetworkException) {
    HttpResult res;
    _objectConfigData->_securityManager->addAccessToken2Req(paramValues);
    res = _objectConfigData->_httpCli->httpPost(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NacosAuthHttpDelegate::httpPut
(
    const NacosString &path,
    std::list<NacosString> &headers,
    std::list<NacosString> &paramValues,
    const NacosString &_encoding,
    long readTimeoutMs
) NACOS_THROW(NetworkException) {
    HttpResult res;
    _objectConfigData->_securityManager->addAccessToken2Req(paramValues);
    res = _objectConfigData->_httpCli->httpPut(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}
}