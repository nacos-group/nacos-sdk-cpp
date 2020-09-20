#ifndef __DIR_UTILS_H_
#define __DIR_UTILS_H_

#include "NacosString.h"

class DirUtils {
public:
    static NacosString getHome();

    static NacosString getCwd();
};

#endif