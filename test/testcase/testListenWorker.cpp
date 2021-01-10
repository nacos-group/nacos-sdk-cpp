#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "src/log/Logger.h"
#include "DebugAssertion.h"
#include "listen/Listener.h"
#include "src/http/HttpDelegate.h"
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "constant/PropertyKeyConst.h"

using namespace std;
using namespace nacos;

bool key_change_called = false;

class KeyChangeListener : public Listener {
private:
    NacosString key;
public:
    void setKey(const NacosString &_key) { key = _key; };

    NacosString getKey() const { return key; };

    void receiveConfigInfo(const NacosString &configInfo) {
        key_change_called = true;
        cout << "in receiveConfigInfo: " << key << " changed to " << configInfo << endl;
    }
};

bool testAddListener() {
    cout << "in function testAddListener" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    ADD_AUTH_INFO(props);
    KeyChangeListener *thelistener = new KeyChangeListener();
    thelistener->setKey("k");
    bool bSucc;
    try {
        NacosServiceFactory *factory = new NacosServiceFactory(props);
        ResourceGuard <NacosServiceFactory> _guardFactory(factory);
        ConfigService *n = factory->CreateConfigService();
        ResourceGuard <ConfigService> _serviceFactory(n);
        n->removeConfig("k4", NULLSTR);
        n->removeConfig("k2", NULLSTR);//Known issue: this key will be monitored in next cycle(LONG_PULLING_TIME)
        n->removeConfig("k", NULLSTR);//so will this
        sleep(1);
        n->addListener("k4", NULLSTR, thelistener);
        n->addListener("k2", NULLSTR, thelistener);
        n->addListener("k", NULLSTR, thelistener);
        sleep(1);
        cout << "publish k" << endl;
        bSucc = n->publishConfig("k4", NULLSTR, "hahaha");
    }
    catch (NacosException &e) {
        cout <<
             "Failed to add listener" << endl <<
             "Reason:" << e.what() << endl;
        return false;
    }

    SHOULD_BE_TRUE(key_change_called, "Notification function should be called");
    SHOULD_BE_TRUE(bSucc, "Publish should succeed");
    cout << "test successful" << endl;
    return true;
}