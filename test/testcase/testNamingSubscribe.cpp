#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "naming/subscribe/EventListener.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;

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
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    NamingService *n = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(n);

    n->subscribe("ss", new MyServiceListener(1));
    cout << "Press any key to register services" << endl;
    getchar();

    n->registerInstance("ss", "127.0.0.1", 33);
    n->registerInstance("ss", "127.0.0.1", 34);
    cout << "Press any key to deregister services" << endl;
    getchar();

    n->deregisterInstance("ss", "127.0.0.1", 33);
    n->deregisterInstance("ss", "127.0.0.1", 34);
    cout << "All instances Unregistered, press any key to finish testing" << endl;
    getchar();

    return true;
}