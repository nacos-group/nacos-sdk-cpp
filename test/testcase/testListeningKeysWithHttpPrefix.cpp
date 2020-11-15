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

class MyListenerHttpPrefix : public Listener {
private:
    int num;
public:
    MyListenerHttpPrefix(int num) {
        this->num = num;
    }

    void receiveConfigInfo(const NacosString &configInfo) {
        cout << "===================================" << endl;
        cout << "Watcher" << num << endl;
        cout << "Watched Key UPDATED:" << configInfo << endl;
        cout << "===================================" << endl;
    }
};

bool testListeningKeysWithHttpPrefix() {
    cout << "in function testListeningKeysWithHttpPrefix" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "HttP://127.0.0.1:8848,HtTP://localhost";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);

    MyListenerHttpPrefix *theListener = new MyListenerHttpPrefix(1);
    MyListenerHttpPrefix *theListener2 = new MyListenerHttpPrefix(2);
    MyListenerHttpPrefix *theListener3 = new MyListenerHttpPrefix(3);
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

    cout << "Hold for 20 secs" << endl;
    sleep(20);
    cout << "remove listener" << endl;
    n->removeListener("dqid", NULLSTR, theListener);

    cout << "Hold for 20 secs" << endl;
    sleep(20);
    cout << "remove listener2" << endl;
    n->removeListener("dqid", NULLSTR, theListener2);
    n->removeListener("dqid", NULLSTR, theListener3);
    cout << "test successful" << endl;

    return true;
}