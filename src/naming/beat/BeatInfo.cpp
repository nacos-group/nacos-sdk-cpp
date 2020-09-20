#include "json/JSON.h"
#include "naming/beat/BeatInfo.h"
#include "NacosString.h"

NacosString BeatInfo::toString() {
    return JSON::toJSONString(*this);
}