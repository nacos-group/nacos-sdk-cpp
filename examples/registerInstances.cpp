#include <iostream>
#include <unistd.h>
#include "Nacos.h"

using namespace std;
using namespace nacos;

int main() {
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(configProps);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingService *namingSvc = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(namingSvc);
    Instance instance;
    instance.clusterName = "DefaultCluster";
    instance.ip = "127.0.0.1";
    instance.port = 2333;
    instance.instanceId = "1";
    instance.ephemeral = true;

    //Registers 5 services named TestNamingService1...5
    try {
        for (int i = 0; i < 5; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);
            instance.port = 2000 + i;
            namingSvc->registerInstance(serviceName, instance);
        }
    }
    catch (NacosException &e) {
        cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
        return -1;
    }
    sleep(30);
    try {
        for (int i = 0; i < 5; i++) {
            NacosString serviceName = "TestNamingService" + NacosStringOps::valueOf(i);

            namingSvc->deregisterInstance(serviceName, "127.0.0.1", 2000 + i);
            sleep(1);
        }
    }
    catch (NacosException &e) {
        cout << "encounter exception while registering service instance, raison:" << e.what() << endl;
        return -1;
    }
    sleep(30);

    return 0;
}
