#include <iostream>
#include "src/config/AppConfigManager.h"
#include "constant/PropertyKeyConst.h"
#include "utils/DirUtils.h"
#include "constant/ConfigConstant.h"

using namespace std;
using namespace nacos;

bool testAppConfigManager() {
    cout << "in function testAppConfigManager" << endl;

    NacosString configFile = DirUtils::getCwd() + ConfigConstant::FILE_SEPARATOR + PropertyKeyConst::CONFIG_FILE_NAME;
    AppConfigManager appConfigManager(configFile);
    appConfigManager.loadConfig(configFile);
    Properties configs = appConfigManager.getAllConfig();
    return true;
}