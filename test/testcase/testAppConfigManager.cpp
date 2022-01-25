#include <iostream>
#include "src/config/AppConfigManager.h"
#include "src/utils/DirUtils.h"
#include "constant/ConfigConstant.h"

using namespace std;
using namespace nacos;

bool testAppConfigManager() {
    cout << "in function testAppConfigManager" << endl;

    NacosString configFile = DirUtils::getCwd() + ConfigConstant::FILE_SEPARATOR + ConfigConstant::DEFAULT_CONFIG_FILE;
    AppConfigManager appConfigManager(configFile);
    appConfigManager.loadConfig(configFile);
    Properties configs = appConfigManager.getAllConfig();
    return true;
}