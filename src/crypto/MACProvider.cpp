//
// Created by liuhanyu on 2021/7/8.
//

#include "MACProvider.h"
#include "src/crypto/hmac_sha1/hmac/hmac.h"

namespace nacos {

const NacosString MACProvider::HMAC_SHA1 = "HMAC_SHA1";
std::map<NacosString, IMAC*> MACProvider::MACRegistry;

class HMACSha1 : public IMAC {
public:
    void getMac(const void *k,   /* secret key */
                size_t lk,  /* length of the key in bytes */
                const void *d,   /* data */
                size_t ld,  /* length of data in bytes */
                void *out, /* output buffer, at least "t" bytes */
                size_t *t);
};

void HMACSha1::getMac(const void *k,   /* secret key */
                      size_t lk,  /* length of the key in bytes */
                      const void *d,   /* data */
                      size_t ld,  /* length of data in bytes */
                      void *out, /* output buffer, at least "t" bytes */
                      size_t *t) {
    hmac_sha1((const uint8_t*)k, lk, (const uint8_t*)d, ld, (uint8_t*)out, t);
}

void MACProvider::Init() {
    MACRegistry[HMAC_SHA1] = new HMACSha1();
}

void MACProvider::DeInit() {
    for (std::map<NacosString, IMAC*>::iterator it = MACRegistry.begin(); it != MACRegistry.end(); it++) {
        delete it->second;
    }
    MACRegistry.clear();
}

IMAC *MACProvider::getMAC(const NacosString &algorithm) {
    if (MACRegistry.count(algorithm) > 0) {
        return MACRegistry[algorithm];
    }

    return NULL;
}

}
