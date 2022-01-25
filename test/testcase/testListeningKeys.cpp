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

bool testListeningKeys() {
    cout << "in function testListeningKeys" << endl;
    Properties props;
    ADD_AUTH_INFO(props);
    ADD_SPAS_INFO(props);
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);

    MyListener *theListener = new MyListener(1);
    MyListener *theListener2 = new MyListener(2);
    MyListener *theListener3 = new MyListener(3);
    n->addListener("dqid", NULLSTR, theListener);
    n->addListener("dqid", NULLSTR, theListener2);
    n->addListener("dqid", NULLSTR, theListener3);
    n->addListener("dqid1", NULLSTR, theListener3);
    n->addListener("dqid2", NULLSTR, theListener3);
    n->addListener("dqid3", NULLSTR, theListener3);

    for (int i = 10; i < 60; i++) {
        NacosString strKey = "dqid" + NacosStringOps::valueOf(i);
        n->addListener(strKey, NULLSTR, theListener3);
    }

    cout << "Change key and hold for 15 secs" << endl;
    n->publishConfig("dqid", NULLSTR, "Hello");
    n->publishConfig("dqid", NULLSTR, "World");
    sleep(15);
    cout << "remove listener" << endl;
    n->removeListener("dqid", NULLSTR, theListener);

    cout << "Hold for 15 secs" << endl;
    n->publishConfig("dqid3", NULLSTR, "Hello-3");
    n->publishConfig("dqid3", NULLSTR, "World-3");
    n->publishConfig("dqid1", NULLSTR, "World-3");
    n->publishConfig("dqid2", NULLSTR, "World-3");
    n->publishConfig("dqid3", NULLSTR, "World-3");
    n->publishConfig("dqid1", NULLSTR, "World-3");
    n->publishConfig("dqid2", NULLSTR, "World-3");
    n->publishConfig("dqid3", NULLSTR, "World-3");
    sleep(15);
    cout << "remove listener2" << endl;
    n->publishConfig("dqid", NULLSTR, "Data change before remove");
    n->removeListener("dqid", NULLSTR, theListener2);
    n->removeListener("dqid", NULLSTR, theListener3);
    n->publishConfig("dqid", NULLSTR, "Data change after remove");
    cout << "test successful" << endl;

    return true;
}