#ifndef __NET_UTILS_H_
#define __NET_UTILS_H_

#include "NacosString.h"

namespace nacos{
class NetUtils {
public:
    static NacosString localIP();
};
}//namespace nacos

#endif
