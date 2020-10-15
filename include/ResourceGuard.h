//
// Created by liuhanyu on 2020/9/6.
//

#ifndef NACOS_SDK_CPP_RESOURCEGUARD_H
#define NACOS_SDK_CPP_RESOURCEGUARD_H

#include "Debug.h"

namespace nacos{
template<typename T>
class ResourceGuard {
private:
    T *_obj;//The object being watched
    ResourceGuard();

public:
    ResourceGuard(T *obj) { _obj = obj; };

    ~ResourceGuard() {
        if (_obj != NULL) {
            log_debug("Deconstructing holded resource...\n");
            delete _obj;
            _obj = NULL;
        }
    };
};
}//namespace nacos


#endif //NACOS_SDK_CPP_RESOURCEGUARD_H
