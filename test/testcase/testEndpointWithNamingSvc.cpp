#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "naming/NamingProxy.h"
#include "naming/NacosNamingService.h"
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "naming/Instance.h"
#include "Constants.h"
#include "utils/UtilAndComs.h"
#include "http/HTTPCli.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "Properties.h"
#include "PropertyKeyConst.h"

using namespace std;

bool testEndpointWithNamingProxy() {
    cout << "in function testNamingServiceRegister" << endl;
    Properties configProps;
    configProps[PropertyKeyConst::ENDPOINT] = "127.0.0.1";
    configProps[PropertyKeyConst::ENDPOINT_PORT] = "8848";
    configProps[PropertyKeyConst::CONTEXT_PATH] = "nacos/v1/ns/operator";
    configProps[PropertyKeyConst::CLUSTER_NAME] = "servers";

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
    sleep(30);
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
    sleep(30);

    return true;
}
