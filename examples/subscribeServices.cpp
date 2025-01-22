#include <iostream>
#include "Nacos.h"
#include <stdio.h>

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

int main() {
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    //Interval for poller to check the status of subscribed services(unit:Ms), 30000 by default
    //Here we set it to 5000 to see the output more quick
    props[PropertyKeyConst::SUBSCRIPTION_POLL_INTERVAL] = "5000";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    NamingService *n = factory->CreateNamingService();
    ResourceGuard <NamingService> _serviceFactory(n);

    n->subscribe("ss", new MyServiceListener(1));
    cout << "Press any key to register services" << endl;
    getchar();

    n->registerInstance("ss", "127.0.0.1", 33);
    n->registerInstance("ss", "127.0.0.1", 34);
    cout << "Press any key to deregister services" << endl;
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();

    n->deregisterInstance("ss", "127.0.0.1", 33);
    n->deregisterInstance("ss", "127.0.0.1", 34);
    cout << "All instances Unregistered, press any key to finish testing" << endl;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();

    return 0;
}
