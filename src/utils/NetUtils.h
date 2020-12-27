#ifndef __NET_UTILS_H_
#define __NET_UTILS_H_

#include "NacosString.h"
#include "NacosExceptions.h"

namespace nacos{
class NetUtils {
public:
    //Get IP address (best guess)
    static NacosString getHostIp() throw(NacosException);
};
}//namespace nacos

#endif
