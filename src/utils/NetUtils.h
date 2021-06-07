#ifndef __NET_UTILS_H_
#define __NET_UTILS_H_

#include "NacosString.h"
#include "NacosExceptions.h"
#include "Compatibility.h"

namespace nacos{
class NetUtils {
public:
    //Get IP address (best guess)
    static NacosString getHostIp() NACOS_THROW(NacosException);

    //Get hostname
    static NacosString getHostName() NACOS_THROW(NacosException);
};
}//namespace nacos

#endif
