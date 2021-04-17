#include "NoOpHttpDelegate.h"

using namespace std;

namespace nacos{
NacosString NoOpHttpDelegate::getEncode() const {
    return encoding;
}

NoOpHttpDelegate::NoOpHttpDelegate
(
    ObjectConfigData *objectConfigData
) {
    _objectConfigData = objectConfigData;
}

HttpResult NoOpHttpDelegate::httpGet
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    HttpResult res;

    res = _objectConfigData->_httpCli->httpGet(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NoOpHttpDelegate::httpDelete
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    HttpResult res;
    res = _objectConfigData->_httpCli->httpDelete(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NoOpHttpDelegate::httpPost
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {
    HttpResult res;

    res = _objectConfigData->_httpCli->httpPost(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NoOpHttpDelegate::httpPut
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) NACOS_THROW(NetworkException) {

    HttpResult res;
    res = _objectConfigData->_httpCli->httpPut(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

}//namespace nacos
