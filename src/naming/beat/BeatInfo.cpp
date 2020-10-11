#include "src/json/JSON.h"
#include "BeatInfo.h"
#include "NacosString.h"

NacosString BeatInfo::toString() {
    return JSON::toJSONString(*this);
}