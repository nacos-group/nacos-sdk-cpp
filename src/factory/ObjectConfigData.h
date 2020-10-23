#ifndef __OBJ_CFG_DATA_H_
#define __OBJ_CFG_DATA_H_

#include "src/naming/NamingProxy.h"
#include "naming/NamingService.h"
#include "config/ConfigService.h"
#include "src/config/AppConfigManager.h"
#include "src/naming/beat/BeatReactor.h"

namespace nacos{
class ObjectConfigData {
public:
    NacosString name;
    NamingProxy *namingProxy;
    BeatReactor *beatReactor;
    IHttpCli *httpCli;
    AppConfigManager *appConfigManager;
    ServerListManager *serverListManager;
};
}//namespace nacos

#endif