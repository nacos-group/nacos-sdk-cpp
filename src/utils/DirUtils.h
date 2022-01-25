#ifndef __DIR_UTILS_H_
#define __DIR_UTILS_H_

#include "NacosString.h"

namespace nacos{
class DirUtils {
public:
    static NacosString getHome();

    static NacosString getCwd();
};
}//namespace nacos

#endif