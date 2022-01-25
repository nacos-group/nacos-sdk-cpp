#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <list>
#include "factory/NacosFactoryFactory.h"
#include "naming/NamingMaintainService.h"
#include "src/debug/DebugAssertion.h"
#include "src/log/Logger.h"
#include "NacosString.h"
#include "Properties.h"
#include "constant/PropertyKeyConst.h"
#include "ResourceGuard.h"

using namespace std;
using namespace nacos;

bool testMaintainGetService() {
    cout << "in function testMaintainGetService" << endl;
    Properties configProps;
    ADD_AUTH_INFO(configProps);
    ADD_SPAS_INFO(configProps);
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(configProps);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingMaintainService *maintainService = factory->CreateNamingMaintainService();
    ResourceGuard <NamingMaintainService> _guardService(maintainService);
    NamingService *namingService = factory->CreateNamingService();
    ResourceGuard <NamingService> _guardService2(namingService);

    ListView<NacosString> res;
    try {
        namingService->registerInstance("MaintainTestService", "127.0.0.1", 2333);
        ServiceInfo2 res = maintainService->queryService("MaintainTestService", NULLSTR);
        cout << "service name got from server:" << res.getName() << endl;
        SHOULD_BE_TRUE(res.getName().compare("MaintainTestService") == 0, "Service name should be MaintainTestService");
    }
    catch (NacosException &e) {
        cout << "encounter exception while getting service, raison:" << e.what() << endl;
        return false;
    }

    return true;
}

bool testMaintainUpdateService() {
    cout << "in function testMaintainUpdateService" << endl;
    Properties configProps;
    ADD_AUTH_INFO(configProps);
    ADD_SPAS_INFO(configProps);
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(configProps);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingMaintainService *maintainService = factory->CreateNamingMaintainService();
    ResourceGuard <NamingMaintainService> _guardService(maintainService);
    NamingService *namingService = factory->CreateNamingService();
    ResourceGuard <NamingService> _guardService2(namingService);

    ListView<NacosString> res;
    try {
        namingService->registerInstance("MaintainTestUpdateService", "127.0.0.1", 2333);
        ServiceInfo2 serviceInfo2;
        serviceInfo2.setName("MaintainTestUpdateService");
        serviceInfo2.setProtectThreshold((double)2);
        bool updateRes = maintainService->updateService(serviceInfo2, NULL);
        cout << "update protect threshold:" << NacosStringOps::valueOf(updateRes) << endl;
        SHOULD_BE_TRUE(updateRes, "update of protect threshold should be successful");

        ServiceInfo2 serviceInfoQuery = maintainService->queryService("MaintainTestService", NULLSTR);
        SHOULD_BE_TRUE(serviceInfoQuery.getProtectThreshold() - 2.0 < 1e-9, "protect threshold should be 2.0D");
    }
    catch (NacosException &e) {
        cout << "encounter exception while updating service, raison:" << e.what() << endl;
        return false;
    }

    return true;
}

//actually this will test delete as well
bool testMaintainCreateService() {
    cout << "in function testMaintainCreateService" << endl;
    Properties configProps;
    ADD_AUTH_INFO(configProps);
    ADD_SPAS_INFO(configProps);
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(configProps);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingMaintainService *maintainService = factory->CreateNamingMaintainService();
    ResourceGuard <NamingMaintainService> _guardService(maintainService);

    ListView<NacosString> res;
    try {
        ServiceInfo2 serviceInfo2;
        serviceInfo2.setName("MaintainTestCreateService");
        serviceInfo2.setProtectThreshold((double)2);
        map<NacosString, NacosString> metadata;
        metadata.insert(make_pair("hello", "world"));
        metadata.insert(make_pair("girlfriend", "Liao, Sijia"));
        serviceInfo2.setMetadata(metadata);

        try {
            maintainService->deleteService("MaintainTestCreateService", NULLSTR);
        } catch (exception &ignore) {
            log_debug("designated service does not exist\n");
        }

        bool createResult = maintainService->createService(serviceInfo2, NULL);
        SHOULD_BE_TRUE(createResult, "MaintainTestCreateService should be created");

        ServiceInfo2 serviceInfoQuery = maintainService->queryService("MaintainTestCreateService", NULLSTR);
        SHOULD_BE_TRUE(serviceInfoQuery.getProtectThreshold() - 2.0 < 1e-9, "protect threshold should be 2.0D");
        SHOULD_BE_TRUE(serviceInfoQuery.getName().compare("MaintainTestCreateService") == 0, "name should be MaintainTestCreateService");

        map<NacosString, NacosString> returnedMetadata = serviceInfoQuery.getMetadata();
        SHOULD_BE_TRUE(returnedMetadata.count("hello") == 1, "metadata should contain hello");
        SHOULD_BE_TRUE(returnedMetadata.count("girlfriend") == 1, "metadata should contain girlfriend");
        NacosString hello = returnedMetadata["hello"];
        SHOULD_BE_TRUE(hello.compare("world") == 0 , "hello should be world");
        NacosString girlfriend = returnedMetadata["girlfriend"];
        SHOULD_BE_TRUE(girlfriend.compare("Liao, Sijia") == 0, "girlfriend should be correct");

        bool deleteFunctionality = maintainService->deleteService("MaintainTestCreateService", NULLSTR);
        SHOULD_BE_TRUE(deleteFunctionality, "delete should be successful");

    }
    catch (NacosException &e) {
        cout << "encounter exception while updating service, raison:" << e.what() << endl;
        return false;
    }

    return true;
}
