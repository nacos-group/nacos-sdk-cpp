#include <iostream>
#include "factory/NacosServiceFactory.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "ResourceGuard.h"
#include "Debug.h"

using namespace std;
using namespace nacos;

bool testGetConfig() {
    cout << "in function testGetConfig" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    NacosString ss = "";
    try {
        ss = n->getConfig("k", NULLSTR, 1000);
    }
    catch (NacosException e) {
        cout <<
             "Request failed with curl code:" << e.errorcode() << endl <<
             "Reason:" << e.what() << endl;
        return false;
    }
    cout << ss << endl;

    return true;
}

bool testGetConfigwithDefaultPort() {
    cout << "in function testGetConfigwithDefaultPort" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    NacosString ss = n->getConfig("k", NULLSTR, 1000);
    cout << ss << endl;

    return true;
}

bool testInvalidConfig() {
    cout << "in function testInvalidConfig" << endl;
    Properties props;

    NacosString ss;
    try {
        NacosServiceFactory *factory = new NacosServiceFactory(props);
        ResourceGuard <NacosServiceFactory> _guardFactory(factory);
        ConfigService *n = factory->CreateConfigService();
        ResourceGuard <ConfigService> _serviceFactory(n);
        ss = n->getConfig("k", NULLSTR, 1000);
    }
    catch (NacosException e) {
        NacosString errmsgShouldBe = "endpoint is blank";
        if (errmsgShouldBe == e.what()) {
            return true;
        } else {
            return false;
        }
    }
    cout << ss << endl;

    return false;
}