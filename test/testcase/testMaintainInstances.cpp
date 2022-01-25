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
#include "constant/ConfigConstant.h"

using namespace std;
using namespace nacos;

bool testMaintainUpdateInstance() {
    cout << "in function testMaintainUpdateInstance" << endl;
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
        Instance instance;
        instance.serviceName = "MaintainTestUpdateInstance";
        instance.ip = "127.0.0.1";
        instance.port = 2333;
        instance.ephemeral = false;
        try {
            maintainService->deleteService("MaintainTestUpdateInstance", ConfigConstant::DEFAULT_GROUP);
        } catch (exception &ignore) { /*We may come across service not exist exception, just ignore*/ }
        namingService->registerInstance("MaintainTestUpdateInstance", instance);
        map<NacosString, NacosString> metadata;
        metadata.insert(make_pair("hello", "world"));
        instance.metadata = metadata;
        bool res = maintainService->updateInstance("MaintainTestUpdateInstance", NULLSTR, instance);
        SHOULD_BE_TRUE(res, "updateInstance should succeed");

        list<Instance> instanceList = namingService->getInstanceWithPredicate("MaintainTestUpdateInstance", NULL);
        SHOULD_BE_TRUE(instanceList.size() == 1, "there should be one and only one instance of MaintainTestUpdateInstance");
        map<NacosString, NacosString> &metadataGet = instanceList.begin()->metadata;
        SHOULD_BE_TRUE(metadataGet.count("hello") == 1, "metadata should contain hello");
        SHOULD_BE_TRUE(metadataGet["hello"].compare("world") == 0, "metadata should be {hello: world}");
        try {
            //restore to the initial status
            maintainService->deleteService("MaintainTestUpdateInstance", ConfigConstant::DEFAULT_GROUP);
        } catch (exception &ignore) { /*We may come across service not exist exception, just ignore*/ }
        return true;
    }
    catch (NacosException &e) {
        cout << "encounter exception while testing, raison:" << e.what() << endl;
        return false;
    }
}
