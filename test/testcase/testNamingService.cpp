#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "src/naming/NamingProxy.h"
#include "src/naming/NacosNamingService.h"
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "naming/Instance.h"
#include "constant/ConfigConstant.h"
#include "constant/UtilAndComs.h"
#include "src/http/HTTPCli.h"
#include "DebugAssertion.h"
#include "src/log/Logger.h"
#include "NacosString.h"
#include "Properties.h"
#include "constant/PropertyKeyConst.h"

using namespace std;
using namespace nacos;

bool testNamingProxySmokeTest() {
    cout << "in function testNamingProxySmokeTest" << endl;
    NacosString servers = "127.0.0.1:8848";
    Properties props;
    ADD_AUTH_INFO(props);
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
    catch (NacosException &e) {
        cout << "Exception caught during deregistering service, raison:" << e.what() << endl;

        return false;
    }

    //Register 100 services
    try {
        for (int i = 0; i < 10; i++) {
            NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
            theinstance.serviceName = serviceName;
            namingProxy->registerService(serviceName, ConfigConstant::DEFAULT_GROUP, theinstance);
            sleep(1);
        }
    }
    catch (NacosException &e) {
        cout << "Exception caught during registering service, raison:" << e.what() << endl;

        return false;
    }

    //check whether the data are correct
    for (int i = 0; i < 10; i++) {
        NacosString serviceName = "TestServiceName" + NacosStringOps::valueOf(i);
        NacosString serverlist = namingProxy->queryList(serviceName, ConfigConstant::DEFAULT_GROUP, "TestCluster", 0, false);

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
    catch (NacosException &e) {
        cout << "Exception caught during cleaning the test environment, raison:" << e.what() << endl;

        return false;
    }

    return true;
}

bool testNamingProxyServerHealthy() {
    cout << "in function testNamingProxyServerHealthy" << endl;
    NacosString servers = "127.0.0.1:8848";
    Properties props;
    ADD_AUTH_INFO(props);
    props[PropertyKeyConst::SERVER_ADDR] = servers;
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *n = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(n);
    NacosNamingService *nn = (NacosNamingService *) n;
    NamingProxy *namingProxy = nn->getServerProxy();

    //Create a clean environment
    bool healthy;
    try {
        healthy = namingProxy->serverHealthy();
    }
    catch (NacosException &e) {
        cout << "Exception caught during deregistering service, raison:" << e.what() << endl;

        return false;
    }

    if (healthy) {
        cout << "server healthy" << endl;
    } else {

        cout << "server down" << endl;
    }

    return true;
}

bool testNamingProxyFailOver() {
    cout << "in function testNamingProxyFailOver" << endl;
    return true;
}

class NamingServiceSelector : public naming::selectors::Selector<Instance> {
private:
    int _port;
public:
    void setSelectPort(int port) { _port = port; };
    list<Instance> select(const std::list<Instance> &itemsToSelect) {
        list<Instance> res;
        for (list<Instance>::const_iterator it = itemsToSelect.begin();
        it != itemsToSelect.end(); it++) {
            if ((*it).port == _port) {
                res.push_back(*it);
            }
        }

        return res;
    }
};

bool testNamingServiceRegister() {
    cout << "in function testNamingServiceRegister" << endl;
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    ADD_AUTH_INFO(configProps);
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
        for (int i = 0; i < 3; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
            for (int j = 0; j < 3; j++) {
                instance.clusterName = "DefaultCluster";
                instance.ip = "127.0.0.1";
                instance.port = 20000 + i*10+j;
                instance.instanceId = "1";
                instance.ephemeral = true;
                namingSvc->registerInstance(serviceName, instance);
            }
        }
    }
    catch (NacosException &e) {
        cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
        return false;
    }

    getchar();

    try {
        for (int i = 0; i < 3; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
            for (int j = 0; j < 3; j++) {
                NamingServiceSelector selector;
                selector.setSelectPort(20000 + i*10+j);
                list<Instance> instanceList = namingSvc->getInstanceWithPredicate(serviceName, &selector);
                cout << "Instance info got from server:" << instanceList.begin()->toString() << endl;
                SHOULD_BE_TRUE(instanceList.size() == 1, "There should be only 1 instance for each port");
                SHOULD_BE_TRUE(instanceList.begin()->clusterName == "DefaultCluster", "Cluster name should be DefaultCluster");
                SHOULD_BE_TRUE(instanceList.begin()->ephemeral == true, "Should be ephemeral node");
            }
        }
    }
    catch (NacosException &e) {
        cout << "encounter exception while getting service instance, raison:" << e.what() << endl;
        return false;
    }
    
    getchar();

    cout << "testNamingServiceRegister successful" << endl;

    return true;
}
