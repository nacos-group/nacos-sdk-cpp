#ifndef __NET_UTILS_H_
#define __NET_UTILS_H_

#include "NacosString.h"

class NetUtils {
private:
    static NacosString LOCAL_IP;
public:
    static NacosString localIP();

    static void Init();
};

#endif
