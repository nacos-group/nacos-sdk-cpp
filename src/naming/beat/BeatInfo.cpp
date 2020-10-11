#include "src/json/JSON.h"
#include "BeatInfo.h"
#include "NacosString.h"

namespace nacos{
NacosString BeatInfo::toString() {
    return JSON::toJSONString(*this);
}
}//namespace nacos