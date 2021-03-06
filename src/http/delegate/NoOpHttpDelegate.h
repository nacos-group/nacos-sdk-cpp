#ifndef __SVR_HTTP_AGENT_H_
#define __SVR_HTTP_AGENT_H_

#include "NacosExceptions.h"
#include "NacosString.h"
#include "src/http/HttpDelegate.h"
#include "src/factory/ObjectConfigData.h"

/**
 * NoOpHttpDelegate
 *
 * @author Liu, Hanyu
 * Directly send request to HttpCli without any operation
 */
namespace nacos{
class NoOpHttpDelegate : public HttpDelegate {
private:
    ObjectConfigData *_objectConfigData;
    NacosString encoding;
public:
    NoOpHttpDelegate(ObjectConfigData *objectConfigData);

    HttpResult httpGet(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
                       const NacosString &encoding, long readTimeoutMs) throw(NetworkException);

    HttpResult httpPost(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
                        const NacosString &encoding, long readTimeoutMs) throw(NetworkException);

    virtual HttpResult
    httpPut(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
            const NacosString &encoding, long readTimeoutMs) throw(NetworkException);

    HttpResult
    httpDelete(const NacosString &path, std::list <NacosString> &headers, std::list <NacosString> &paramValues,
               const NacosString &encoding, long readTimeoutMs) throw(NetworkException);

    NacosString getEncode() const;

    virtual ~NoOpHttpDelegate() {
    };
};
}//namespace nacos

#endif