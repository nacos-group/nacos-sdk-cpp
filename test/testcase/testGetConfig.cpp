#include <iostream>
#include "factory/NacosServiceFactory.h"
#include "constant/PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "ResourceGuard.h"
#include "src/log/Logger.h"

using namespace std;
using namespace nacos;

bool testGetConfig() {
    cout << "in function testGetConfig" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    ADD_AUTH_INFO(props);
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    NacosString ss = "";
    try {
        ss = n->getConfig("k", NULLSTR, 1000);
    }
    catch (NacosException &e) {
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
    ADD_AUTH_INFO(props);
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
    ADD_AUTH_INFO(props);

    NacosString ss;
    try {
        NacosServiceFactory *factory = new NacosServiceFactory(props);
        ResourceGuard <NacosServiceFactory> _guardFactory(factory);
        ConfigService *n = factory->CreateConfigService();
        ResourceGuard <ConfigService> _serviceFactory(n);
        ss = n->getConfig("k", NULLSTR, 1000);
    }
    catch (NacosException &e) {
        NacosString errmsgShouldBe = "no server address specified and the endpoint is blank";
        if (errmsgShouldBe == e.what()) {
            return true;
        } else {
            return false;
        }
    }
    cout << ss << endl;

    return false;
}