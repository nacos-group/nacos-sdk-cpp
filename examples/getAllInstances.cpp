#include <iostream>
#include <list>
#include "Nacos.h"

using namespace std;
using namespace nacos;

int main() {
    Properties configProps;
    configProps[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(configProps);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingService *namingSvc = factory->CreateNamingService();
    ResourceGuard <NamingService> _guardService(namingSvc);

    list <Instance> instances = namingSvc->getAllInstances("TestNamingService1");
    cout << "getAllInstances from server:" << endl;
    for (list<Instance>::iterator it = instances.begin();
         it != instances.end(); it++) {
        cout << "Instance:" << it->toString() << endl;
    }

    return 0;
}
