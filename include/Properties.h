#ifndef __PROPERTIES_H_
#define __PROPERTIES_H_

#include <map>
#include "NacosString.h"

namespace nacos{
class Properties : public std::map<NacosString, NacosString> {
public:
    NacosString toString() const {
        NacosString content = "";
        for (std::map<NacosString, NacosString>::const_iterator it = begin(); it != end(); it++) {
            content += (it->first + "=" + it->second + "\n");
        }
        return content;
    }

    bool contains(const NacosString &key) const {
        if (count(key) > 0) {
            return true;
        }

        return false;
    }
};
}//namespace nacos

#endif