#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "factory/NacosServiceFactory.h"
#include "naming/Instance.h"
#include "naming/selectors/HealthInstanceSelector.h"
#include "Constants.h"
#include "utils/UtilAndComs.h"
#include "http/HTTPCli.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"
#include "ResourceGuard.h"

using namespace std;

bool testInstanceSelectors() {
    cout << "in function testInstanceSelectors" << endl;
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    NacosServiceFactory *factory = new NacosServiceFactory(configProps);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *namingSvc = factory->CreateNamingService();
    ResourceGuard <NamingService> _guardService(namingSvc);

    list<Instance> res;
    HealthInstanceSelector healthInstanceSelector;
    try {
        res = namingSvc->getInstanceWithPredicate("ss", &healthInstanceSelector);
    }
    catch (NacosException e) {
        cout << "encounter exception while getting service names, raison:" << e.what() << endl;
        return false;
    }

    for (list<Instance>::const_iterator it = res.begin();
         it != res.end(); it++) {
        cout << "service:" << it->toString() << endl;
    }

    return true;
}