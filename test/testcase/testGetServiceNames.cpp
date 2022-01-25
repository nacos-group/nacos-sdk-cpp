#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "src/naming/NamingProxy.h"
#include "factory/NacosFactoryFactory.h"
#include "naming/Instance.h"
#include "constant/ConfigConstant.h"
#include "constant/UtilAndComs.h"
#include "src/http/HTTPCli.h"
#include "src/debug/DebugAssertion.h"
#include "src/log/Logger.h"
#include "NacosString.h"
#include "Properties.h"
#include "constant/PropertyKeyConst.h"
#include "ResourceGuard.h"

using namespace std;
using namespace nacos;

bool testGetServiceNames() {
    cout << "in function testGetServiceNames" << endl;
    Properties configProps;
    ADD_AUTH_INFO(configProps);
    ADD_SPAS_INFO(configProps);
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(configProps);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingService *namingSvc = factory->CreateNamingService();
    ResourceGuard <NamingService> _guardService(namingSvc);

    ListView<NacosString> res;
    try {
        res = namingSvc->getServiceList(1, 10);
    }
    catch (NacosException &e) {
        cout << "encounter exception while getting service names, raison:" << e.what() << endl;
        return false;
    }

    list<NacosString> nameList = res.getData();
    for (list<NacosString>::const_iterator it = nameList.begin();
        it != nameList.end(); it++) {
        cout << "serviceName:" << *it << endl;
    }

    return true;
}