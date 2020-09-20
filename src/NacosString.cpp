#include <math.h>
#include <stdio.h>
#include <sstream>
#include "NacosString.h"

const NacosString NacosStringOps::nullstr = "";
const NacosString NacosStringOps::TRUE = "true";
const NacosString NacosStringOps::FALSE = "false";

//Returns true if str refers to nullstr
bool NacosStringOps::isNullStr(const NacosString &str) {
    return (&str == &nullstr) || (str == "");
}

template<>
NacosString NacosStringOps::valueOf<bool>(bool val) {
    if (val) { return TRUE; }
    else { return FALSE; }
}