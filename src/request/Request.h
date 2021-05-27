//
// Created by liuhanyu on 2021/5/24.
//

#ifndef NACOS_SDK_CPP_REQUEST_H
#define NACOS_SDK_CPP_REQUEST_H
#include "NacosString.h"
#include <map>

namespace nacos {

class Request {
private:
    NacosString _requestId;
    std::map<NacosString, NacosString> headers;
public:
    virtual ~Request() {};
    virtual NacosString &getModule() = 0;
};

}

#endif //NACOS_SDK_CPP_REQUEST_H
