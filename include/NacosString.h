#ifndef __NACOS_STRING_H_
#define __NACOS_STRING_H_

#include <string>
#include <sstream>

#define NacosString std::string
#define NULLSTR NacosStringOps::nullstr
#define isNull NacosStringOps::isNullStr

namespace nacos{
class NacosStringOps {
public:
    static const NacosString nullstr;

    static bool isNullStr(const NacosString &str);

    template<typename T>
    static NacosString valueOf(T val);

    static const NacosString TRUE;
    static const NacosString FALSE;
};

template<typename T>
NacosString NacosStringOps::valueOf(T val) {
    std::ostringstream os;
    if (os << val) {
        return NacosString(os.str().c_str());
    }

    return NULLSTR;
}
}//namespace nacos

#endif
