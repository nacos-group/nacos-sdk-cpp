#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "factory/NacosFactoryFactory.h"
#include "ResourceGuard.h"
#include "naming/subscribe/EventListener.h"
#include "constant/PropertyKeyConst.h"
#include "src/debug/DebugAssertion.h"
#include "src/log/Logger.h"

using namespace std;
using namespace nacos;

class MyServiceListener : public EventListener {
private:
    int num;
public:
    MyServiceListener(int num) {
        this->num = num;
    }

    void receiveNamingInfo(const ServiceInfo &serviceInfo){
        cout << "===================================" << endl;
        cout << "Watcher: " << num << endl;
        cout << "Watched service UPDATED: " << serviceInfo.toInstanceString() << endl;
        cout << "===================================" << endl;

    }
};

bool testListenService() {
    cout << "in function testListenService" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    props[PropertyKeyConst::LOCAL_IP] = "127.0.0.1";
    ADD_AUTH_INFO(props);
    ADD_SPAS_INFO(props);
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingService *n = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(n);

    n->subscribe("ss", new MyServiceListener(1));

    n->registerInstance("ss", "127.0.0.1", 33);
    n->registerInstance("ss", "127.0.0.1", 34);

    n->deregisterInstance("ss", "127.0.0.1", 33);
    n->deregisterInstance("ss", "127.0.0.1", 34);

    n->registerInstance("ss", "127.0.0.1", 33);
    n->registerInstance("ss", "127.0.0.1", 34);

    n->deregisterInstance("ss", "127.0.0.1", 33);
    n->deregisterInstance("ss", "127.0.0.1", 34);

    cout << "All instances Unregistered" << endl;

    return true;
}

//testcase for bugfix#101, if the problem persists, this function will cause a coredump
bool testSubscribeAlotOfServices() {
    cout << "in function testSubscribeAlotOfServices" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    props[PropertyKeyConst::LOCAL_IP] = "127.0.0.1";
    props[PropertyKeyConst::LOG_LEVEL] = "WARN";
    ADD_AUTH_INFO(props);
    ADD_SPAS_INFO(props);
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingService *n = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(n);

    n->subscribe("ss", new MyServiceListener(1));

    for (int i = 0; i < 1000; i++) {

        n->registerInstance("ss", "127.0.0.1", 2000+i);
    }

    sleep(20);

    for (int i = 0; i < 1000; i++) {
        n->deregisterInstance("ss", "127.0.0.1", 2000+i);
    }

    return true;
}