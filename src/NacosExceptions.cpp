#include "NacosExceptions.h"

namespace nacos{
NacosException::NacosException(int errorcode, const char *errormsg) NACOS_NOTHROW() {
    _errcode = errorcode;
    _errmsg = errormsg;
}

NacosException::NacosException(int errorcode, const NacosString &errormsg) NACOS_NOTHROW() {
    _errcode = errorcode;
    _errmsg = errormsg;
}
}//namespace nacos
