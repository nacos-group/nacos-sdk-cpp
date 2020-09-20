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

bool testNamingProxySmokeTest() {
    cout << "in function testNamingProxySmokeTest" << endl;
    NacosString servers = "127.0.0.1:8848";
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = servers;
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *n = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(n);
    NacosNamingService *nn = (NacosNamingService *) n;
    NamingProxy *namingProxy = nn->getServerProxy();
    Instance theinstance;
    theinstance.instanceId = "TestInstance";
    theinstance.ip = "127.0.0.1";
    theinstance.port = 3333;
    theinstance.clusterName = "TestCluster";

    //Create a clean environment
    try {
        for (int i = 0; i < 10; i++) {
            NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
            theinstance.serviceName = serviceName;
            namingProxy->deregisterService(serviceName, theinstance);
        }
    }
    catch (NacosException e) {
        cout << "Exception caught during deregistering service, raison:" << e.what() << endl;

        return false;
    }

    //Register 100 services
    try {
        for (int i = 0; i < 10; i++) {
            NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
            theinstance.serviceName = serviceName;
            namingProxy->registerService(serviceName, Constants::DEFAULT_GROUP, theinstance);
            sleep(1);
        }
    }
    catch (NacosException e) {
        cout << "Exception caught during registering service, raison:" << e.what() << endl;

        return false;
    }

    //check whether the data are correct
    for (int i = 0; i < 10; i++) {
        NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
        NacosString serverlist = namingProxy->queryList(serviceName, "TestCluster", 0, false);

        if (serverlist.find("\"serviceName\":\"" + serviceName + "\"") == string::npos) {
            cout << "Failed to get data for:" << serviceName << endl;
            return false;
        }
        cout << "Servers from nacos:" + serverlist << endl;
    }

    //Clear-ups
    try {
        for (int i = 0; i < 10; i++) {
            NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
            theinstance.serviceName = serviceName;
            namingProxy->deregisterService(serviceName, theinstance);
        }
    }
    catch (NacosException e) {
        cout << "Exception caught during cleaning the test environment, raison:" << e.what() << endl;

        return false;
    }

    return true;
}

bool testNamingProxyFailOver() {
    cout << "in function testNamingProxyFailOver" << endl;
    return true;
}

bool testNamingServiceRegister() {
    cout << "in function testNamingServiceRegister" << endl;
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
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
