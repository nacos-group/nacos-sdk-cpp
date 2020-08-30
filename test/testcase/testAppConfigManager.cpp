#include <iostream>
#include "config/AppConfigManager.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "utils/DirUtils.h"
#include "Constants.h"

using namespace std;

bool testAppConfigManager()
{
	cout << "in function testAppConfigManager" << endl;

	AppConfigManager appConfigManager;
    NacosString configFile = DirUtils::getCwd() + Constants::FILE_SEPARATOR + PropertyKeyConst::CONFIG_FILE_NAME;
    appConfigManager.loadConfig(configFile);
    Properties configs = appConfigManager.getAllConfig();
	return true;
}