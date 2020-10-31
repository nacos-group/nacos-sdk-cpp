#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "factory/NacosServiceFactory.h"
#include "naming/NamingMaintainService.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"
#include "ResourceGuard.h"

using namespace std;
using namespace nacos;

bool testMaintainGetService() {
    cout << "in function testGetService" << endl;
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    NacosServiceFactory *factory = new NacosServiceFactory(configProps);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingMaintainService *maintainService = factory->CreateNamingMaintainService();
    ResourceGuard <NamingMaintainService> _guardService(maintainService);
    NamingService *namingService = factory->CreateNamingService();
    ResourceGuard <NamingService> _guardService2(namingService);

    ListView<NacosString> res;
    try {
        namingService->registerInstance("MaintainTestService", "127.0.0.1", 2333);
        ServiceInfo2 res = maintainService->queryService("MaintainTestService", NULLSTR);
        cout << res.getName() << endl;
    }
    catch (NacosException e) {
        cout << "encounter exception while getting service, raison:" << e.what() << endl;
        return false;
    }

    return true;
}