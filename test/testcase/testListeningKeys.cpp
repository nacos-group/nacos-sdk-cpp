#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "listen/Listener.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"

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
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
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

    cout << "Hold for 2 mins" << endl;
    sleep(120);
    cout << "remove listener" << endl;
    n->removeListener("dqid", NULLSTR, theListener);

    cout << "Hold for 2 mins" << endl;
    sleep(120);
    cout << "remove listener2" << endl;
    n->removeListener("dqid", NULLSTR, theListener2);
    n->removeListener("dqid", NULLSTR, theListener3);
    cout << "test successful" << endl;

    return true;
}