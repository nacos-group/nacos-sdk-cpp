#include <string.h>
#include "NoOpHttpDelegate.h"
#include "Debug.h"

using namespace std;

namespace nacos{
NacosString NoOpHttpDelegate::getEncode() const {
    return encoding;
}

NoOpHttpDelegate::NoOpHttpDelegate
(
        IHttpCli *httpcli,
        const NacosString &_encoding
) {
    encoding = _encoding;
    httpCli = httpcli;
}

HttpResult NoOpHttpDelegate::httpGet
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) throw(NetworkException) {
    HttpResult res;

    res = httpCli->httpGet(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NoOpHttpDelegate::httpDelete
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) throw(NetworkException) {
    HttpResult res;
    res = httpCli->httpDelete(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NoOpHttpDelegate::httpPost
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) throw(NetworkException) {
    HttpResult res;

    res = httpCli->httpPost(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

HttpResult NoOpHttpDelegate::httpPut
        (
                const NacosString &path,
                std::list <NacosString> &headers,
                std::list <NacosString> &paramValues,
                const NacosString &_encoding,
                long readTimeoutMs
        ) throw(NetworkException) {

    HttpResult res;
    res = httpCli->httpPut(path, headers, paramValues, _encoding, readTimeoutMs);
    return res;
}

}//namespace nacos
