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

bool testMaintainUpdateInstance() {
    cout << "in function testMaintainUpdateInstance" << endl;
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
        namingService->registerInstance("MaintainTestInstance", "127.0.0.1", 2333);
        Instance instance;
        instance.serviceName = "MaintainTestInstance";
        instance.ip = "127.0.0.1";
        instance.port = 9999;
        bool res = maintainService->updateInstance("MaintainTestInstance", NULLSTR, instance);
        SHOULD_BE_TRUE(res, "updateInstance should succeed");

        list<Instance> instanceList = namingService->getInstanceWithPredicate("MaintainTestInstance", NULL);
        for (list<Instance>::iterator it = instanceList.begin();
            it != instanceList.end(); it++) {
            if (it->port == 9999) {
                cout << "found an instance with port 9999" << endl;
                return true;
            }
        }

        SHOULD_BE_TRUE(false, "cannot find an instance with port 9999, which means the modification failed");
        return false;
    }
    catch (NacosException e) {
        cout << "encounter exception while getting service, raison:" << e.what() << endl;
        return false;
    }

    return true;
}
