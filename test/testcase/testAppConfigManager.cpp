#include <iostream>
#include "src/config/AppConfigManager.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "utils/DirUtils.h"
#include "Constants.h"

using namespace std;
using namespace nacos;

bool testAppConfigManager() {
    cout << "in function testAppConfigManager" << endl;

    NacosString configFile = DirUtils::getCwd() + Constants::FILE_SEPARATOR + PropertyKeyConst::CONFIG_FILE_NAME;
    AppConfigManager appConfigManager(configFile);
    appConfigManager.loadConfig(configFile);
    Properties configs = appConfigManager.getAllConfig();
    return true;
}