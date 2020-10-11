#ifndef __OBJ_CFG_DATA_H_
#define __OBJ_CFG_DATA_H_

#include "src/naming/NamingProxy.h"
#include "naming/NamingService.h"
#include "config/ConfigService.h"
#include "src/config/AppConfigManager.h"
#include "src/naming/beat/BeatReactor.h"

class ObjectConfigData {
public:
    NacosString name;
    NamingProxy *namingProxy;
    BeatReactor *beatReactor;
    HTTPCli *httpCli;
    AppConfigManager *appConfigManager;
    ServerListManager *serverListManager;
};

#endif