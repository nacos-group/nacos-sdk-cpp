#include <math.h>
#include <stdio.h>
#include <sstream>
#include "NacosString.h"

namespace nacos{
const NacosString NacosStringOps::nullstr = "";
const NacosString NacosStringOps::STR_TRUE = "true";
const NacosString NacosStringOps::STR_FALSE = "false";

//Returns true if str refers to nullstr
bool NacosStringOps::isNullStr(const NacosString &str) {
    return (&str == &nullstr) || (str == "");
}

template<>
NacosString NacosStringOps::valueOf<bool>(bool val) {
    if (val) { return STR_TRUE; }
    else { return STR_FALSE; }
}
}//namespace nacos
