//
// Created by liuhanyu on 2021/7/8.
//

#include "MACProvider.h"
#include "src/crypto/hmac_sha1/hmac/hmac.h"

namespace nacos {

std::map<int, IMAC*> *MACProvider::MACRegistry;
const int MACProvider::HMAC_SHA1;

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
    MACRegistry = new std::map<int, IMAC*>();
    (*MACRegistry)[MACProvider::HMAC_SHA1] = new HMACSha1();
}

void MACProvider::DeInit() {
    for (std::map<int, IMAC*>::iterator it = MACRegistry->begin(); it != MACRegistry->end(); it++) {
        IMAC * curMACProvider = it->second;
        delete curMACProvider;
    }

    delete MACRegistry;
}

IMAC *MACProvider::getMAC(int algorithm) {
    if (MACRegistry->count(algorithm) > 0) {
        return (*MACRegistry)[algorithm];
    }

    return NULL;
}

}
