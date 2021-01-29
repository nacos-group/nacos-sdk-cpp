//
// Created by liuhanyu on 2020/12/5.
//

#ifndef NACOS_SDK_CPP_NACOSAUTHHTTPDELEGATE_H
#define NACOS_SDK_CPP_NACOSAUTHHTTPDELEGATE_H

#include "NacosExceptions.h"
#include "NacosString.h"
#include "src/http/HttpDelegate.h"
#include "src/factory/ObjectConfigData.h"

/**
 * NoOpHttpDelegate
 *
 * @author Liu, Hanyu
 * Send a request to the server with authentication header
 */
namespace nacos{
    class NacosAuthHttpDelegate : public HttpDelegate {
    private:
        ObjectConfigData *_objectConfigData;
        NacosString encoding;
    public:
        NacosAuthHttpDelegate(ObjectConfigData *objectConfigData);

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

        virtual ~NacosAuthHttpDelegate() {
        };
    };
}//namespace nacos

#endif //NACOS_SDK_CPP_NACOSAUTHHTTPDELEGATE_H
