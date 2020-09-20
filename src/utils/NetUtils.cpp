#include "utils/NetUtils.h"

NacosString NetUtils::LOCAL_IP;

void NetUtils::Init() {
    LOCAL_IP = "127.0.0.1";//TODO:read local ip;
}

NacosString NetUtils::localIP() {
    return LOCAL_IP;
}
