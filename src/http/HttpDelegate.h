#ifndef __HTTP_AGENT_H_
#define __HTTP_AGENT_H_

#include "NacosExceptions.h"
#include "NacosString.h"
#include "IHttpCli.h"

/**
 * HttpDelegate
 *
 * @author Liu, Hanyu
 */
namespace nacos{
class HttpDelegate {
public:
    /**
    * invoke http get method
    * @param path http path
    * @param headers http headers
    * @param paramValues http paramValues http
    * @param encoding http encode
    * @param readTimeoutMs http timeout
    * @return HttpResult http response
    * @throws NetworkException If an input or output exception occurred
    */

    virtual HttpResult
    httpGet(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
            const NacosString &encoding, long readTimeoutMs) throw(NetworkException) = 0;

    /**
    * invoke http put method
    * @param path http path
    * @param headers http headers
    * @param paramValues http paramValues http
    * @param encoding http encode
    * @param readTimeoutMs http timeout
    * @return HttpResult http response
    * @throws NetworkException If an input or output exception occurred
    */
    virtual HttpResult
    httpPut(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
            const NacosString &encoding, long readTimeoutMs) throw(NetworkException) = 0;

    /**
    * invoke http post method
    * @param path http path
    * @param headers http headers
    * @param paramValues http paramValues http
    * @param encoding http encode
    * @param readTimeoutMs http timeout
    * @return HttpResult http response
    * @throws NetworkException If an input or output exception occurred
    */
    virtual HttpResult
    httpPost(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
             const NacosString &encoding, long readTimeoutMs) throw(NetworkException) = 0;

    /**
    * invoke http delete method
    * @param path http path
    * @param headers http headers
    * @param paramValues http paramValues http
    * @param encoding http encode
    * @param readTimeoutMs http timeout
    * @return HttpResult http response
    * @throws NetworkException If an input or output exception occurred
    */
    virtual HttpResult
    httpDelete(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
               const NacosString &encoding, long readTimeoutMs) throw(NetworkException) = 0;

    /**
    * get encode
    * @return NacosString
    */
    virtual NacosString getEncode() const = 0;

    virtual ~HttpDelegate() {};
};
}//namespace nacos

#endif