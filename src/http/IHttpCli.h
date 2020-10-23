#ifndef __IHTTP_CLI_H_
#define __IHTTP_CLI_H_

#include <list>
#include <map>
#include <curl/curl.h>
#include "NacosExceptions.h"
#include "NacosString.h"

/**
 * HttpDelegate
 *
 * @author Liu, Hanyu
 */
namespace nacos{
class HttpResult {
public:
    long code;
    NacosString content;
    std::map <NacosString, NacosString> headers;
    CURLcode curlcode;

    HttpResult(long _code, const NacosString &_content, std::map <NacosString, NacosString> &_headers)
            : code(_code), content(_content) {
        headers.insert(_headers.begin(), _headers.end());
    }

    HttpResult(long _code, const NacosString &_content) : code(_code), content(_content) {}

    HttpResult() {
        code = -1;
        content = "";
        headers.clear();
    }

    HttpResult operator=(HttpResult asignee) {
        if (this != &asignee) {
            headers.insert(asignee.headers.begin(), asignee.headers.end());
            code = asignee.code;
            content = asignee.content;
            curlcode = asignee.curlcode;
        }

        return *this;
    }
};

class IHttpCli {
public:
    static const int GET = 0;
    static const int PUT = 1;
    static const int POST = 3;
    static const int DELETE = 4;

    virtual HttpResult httpGet(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual HttpResult httpGet(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual HttpResult httpDelete(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual HttpResult httpDelete(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual HttpResult httpPost(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual HttpResult httpPost(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual HttpResult httpPut(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual HttpResult httpPut(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) throw(NetworkException) = 0;

    virtual ~IHttpCli() {};
};
}//namespace nacos

#endif