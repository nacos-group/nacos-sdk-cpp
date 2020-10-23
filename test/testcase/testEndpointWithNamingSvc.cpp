#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "src/naming/NamingProxy.h"
#include "src/naming/NacosNamingService.h"
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "naming/Instance.h"
#include "Constants.h"
#include "utils/UtilAndComs.h"
#include "src/http/HTTPCli.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"

using namespace std;
using namespace nacos;

bool testEndpointWithNamingProxy() {
    cout << "in function testNamingServiceRegister" << endl;
    cout << "For this test, please create an endpoint on your 80 port with a file in the following path:" << endl;
    cout << "yourip:80/endpoints/endpoint0" << endl;
    cout << "And the content should be a list of ip:port separated with \\n the ip:port group points at a nacos server" << endl;
    Properties configProps;
    configProps[PropertyKeyConst::ENDPOINT] = "127.0.0.1";
    configProps[PropertyKeyConst::ENDPOINT_PORT] = "80";
    configProps[PropertyKeyConst::CONTEXT_PATH] = "endpoints";
    configProps[PropertyKeyConst::CLUSTER_NAME] = "endpoint0";

    NacosServiceFactory *factory = new NacosServiceFactory(configProps);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *namingSvc = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(namingSvc);
    Instance instance;
    instance.clusterName = "DefaultCluster";
    instance.ip = "127.0.0.1";
    instance.port = 2333;
    instance.instanceId = "1";
    instance.ephemeral = true;

    try {
        for (int i = 0; i < 5; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
            instance.port = 2000 + i;
            namingSvc->registerInstance(serviceName, instance);
        }
    }
    catch (NacosException e) {
        cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
        return false;
    }
    cout << "Keep the services for 30 secs..." << endl;
    sleep(30);
    cout << "Deregister the services" << endl;
    try {
        for (int i = 0; i < 5; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);

            namingSvc->deregisterInstance(serviceName, "127.0.0.1", 2000 + i);
            sleep(1);
        }
    }
    catch (NacosException e) {
        cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
        return false;
    }

    cout << "testNamingServiceRegister finished" << endl;

    return true;
}
