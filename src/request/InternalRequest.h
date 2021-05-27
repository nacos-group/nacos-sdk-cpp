//
// Created by liuhanyu on 2021/5/24.
//

#ifndef NACOS_SDK_CPP_INTERNALREQUEST_H
#define NACOS_SDK_CPP_INTERNALREQUEST_H
#include "Request.h"

namespace nacos {

class InternalRequest : public Request {

    NacosString &getModule() { return "internal"; };
    virtual ~InternalRequest() {};
};

}

#endif //NACOS_SDK_CPP_INTERNALREQUEST_H
