#ifndef __HTTP_CLI_H_
#define __HTTP_CLI_H_


#include <pthread.h>
#include <curl/curl.h>
#include "NacosString.h"
#include "NacosExceptions.h"
#include "IHttpCli.h"
#include "Compatibility.h"

namespace nacos{

class HTTPCli : public IHttpCli {
private:
    //CURL *curlHandle;
    pthread_key_t pthreadKey;

    CURL *getCurlHandle();

    static void destroyCurlHandle(void *arg);

    HttpResult httpGetInternal
            (
                    const NacosString &path,
                    std::list <NacosString> &headers,
                    const NacosString &paramValues,
                    const NacosString &encoding,
                    long readTimeoutMs
            ) NACOS_THROW(NetworkException);

    HttpResult httpPostInternal
            (
                    const NacosString &path,
                    std::list <NacosString> &headers,
                    const NacosString &paramValues,
                    const NacosString &encoding,
                    long readTimeoutMs
            ) NACOS_THROW(NetworkException);

    HttpResult httpPutInternal
            (
                    const NacosString &path,
                    std::list <NacosString> &headers,
                    const NacosString &paramValues,
                    const NacosString &encoding,
                    long readTimeoutMs
            ) NACOS_THROW(NetworkException);

    HttpResult httpDeleteInternal
            (
                    const NacosString &path,
                    std::list <NacosString> &headers,
                    const NacosString &paramValues,
                    const NacosString &encoding,
                    long readTimeoutMs
            ) NACOS_THROW(NetworkException);

public:
    static NacosString encodingParams(std::list <NacosString> &params);

    static NacosString encodingParams(std::map <NacosString, NacosString> &params);

    static void assembleHeaders(std::list <NacosString> &assembledHeaders, std::list <NacosString> &headers);

    static void HTTPBasicSettings(CURL *curlHandle);

    static void HTTP_GLOBAL_INIT();

    static void HTTP_GLOBAL_DEINIT();

    HTTPCli();

    ~HTTPCli();

    HttpResult httpGet(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

    HttpResult httpGet(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

    HttpResult httpDelete(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

    HttpResult httpDelete(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

    HttpResult httpPost(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

    HttpResult httpPost(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

    HttpResult httpPut(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::list <NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

    HttpResult httpPut(
            const NacosString &path,
            std::list <NacosString> &headers,
            std::map <NacosString, NacosString> &paramValues,
            const NacosString &encoding,
            long readTimeoutMs
    ) NACOS_THROW(NetworkException);

};
}//namespace nacos

#endif
