//
// Created by liuhanyu on 2021/7/8.
//

#ifndef NACOS_SDK_CPP_MACPROVIDER_H
#define NACOS_SDK_CPP_MACPROVIDER_H
#include "NacosString.h"
#include <map>

namespace nacos {

class IMAC {
public:
    virtual void getMac(const void *k,   /* secret key */
                size_t lk,  /* length of the key in bytes */
                const void *d,   /* data */
                size_t ld,  /* length of data in bytes */
                void *out, /* output buffer, at least "t" bytes */
                size_t *t) = 0;
    virtual ~IMAC() {};
};

class MACProvider {
private:
    static std::map<int, IMAC*> *MACRegistry;
public:
    static void Init();
    static void DeInit();
    static IMAC *getMAC(int algorithm);
    static const int HMAC_SHA1 = 930620;
};

}

#endif //NACOS_SDK_CPP_MACPROVIDER_H
