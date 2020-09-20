#ifndef __OBJ_CFG_DATA_H_
#define __OBJ_CFG_DATA_H_

#include "naming/NamingProxy.h"
#include "naming/NamingService.h"
#include "config/ConfigService.h"
#include "config/AppConfigManager.h"
#include "naming/beat/BeatReactor.h"

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