//
// Created by liuhanyu on 2021/7/8.
//

#ifndef SIGNATURE_TOOL
#define SIGNATURE_TOOL

#include "NacosString.h"
#include "MACProvider.h"
#include "base64/base64.h"
#include "src/debug/DebugAssertion.h"

namespace nacos {

/**
 * SignatureTool
 *
 * @author Liu, Hanyu
 * Signature tool
 */
class SignatureTool {
public:
    //Returns a base64-encoded signature string
    static NacosString SignWithHMAC_SHA1(const NacosString &dataToSign, const NacosString &secretKey) {
        IMAC *digester = MACProvider::getMAC(MACProvider::HMAC_SHA1);
        unsigned char signature[20];
        size_t outlen = sizeof(signature);
        digester->getMac(secretKey.c_str(), secretKey.length(), dataToSign.c_str(), dataToSign.length(), (void*)signature, &outlen);
        NACOS_ASSERT(outlen == 20);//must be 20 since we're using HMAC_SHA1
        NacosString encoded_signature = base64_encode(signature, sizeof(signature));
        return encoded_signature;
    }
};

}
#endif //NACOS_SDK_CPP_MACPROVIDER_H
