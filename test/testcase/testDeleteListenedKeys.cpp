#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "factory/NacosFactoryFactory.h"
#include "ResourceGuard.h"
#include "listen/Listener.h"
#include "constant/PropertyKeyConst.h"
#include "src/debug/DebugAssertion.h"
#include "src/log/Logger.h"

using namespace std;
using namespace nacos;

class MyListener : public Listener {
private:
    int num;
public:
    MyListener(int num) {
        this->num = num;
    }

    void receiveConfigInfo(const NacosString &configInfo) {
        cout << "===================================" << endl;
        cout << "Watcher" << num << endl;
        cout << "Watched Key UPDATED:" << configInfo << endl;
        cout << "===================================" << endl;
    }
};

bool testRemoveKeyBeingWatched() {
    cout << "in function testRemoveKeyBeingWatched" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    ADD_AUTH_INFO(props);
    ADD_SPAS_INFO(props);
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    n->publishConfig("RemovedWhileWatching", NULLSTR, "dummyContent");

    MyListener *theListener = new MyListener(1);
    n->addListener("RemovedWhileWatching", NULLSTR, theListener);

    sleep(2);
    cout << "remove key" << endl;
    n->removeConfig("RemovedWhileWatching", NULLSTR);
    sleep(2);
    cout << "set key" << endl;
    n->publishConfig("RemovedWhileWatching", NULLSTR, "dummyContent1");
    sleep(2);
    cout << "remove key" << endl;
    n->removeConfig("RemovedWhileWatching", NULLSTR);
    cout << "Hold for 30 secs" << endl;
    sleep(30);
    n->removeListener("RemovedWhileWatching", NULLSTR, theListener);
    cout << "remove listener2" << endl;
    cout << "test successful" << endl;

    return true;
}