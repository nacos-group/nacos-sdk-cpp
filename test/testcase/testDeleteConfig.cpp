#include <iostream>
#include <stdio.h>
#include "factory/NacosFactoryFactory.h"
#include "constant/PropertyKeyConst.h"
#include "src/debug/DebugAssertion.h"
#include "ResourceGuard.h"

using namespace std;
using namespace nacos;

bool testDeleteConfig() {
    cout << "in function testDeleteConfig" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    ADD_AUTH_INFO(props);
    ADD_SPAS_INFO(props);
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    bool bSucc;
    for (int i = 5; i < 50; i++) {
        char key_s[200];
        char val_s[200];
        snprintf(key_s, sizeof(key_s), "Key%d", i);
        snprintf(val_s, sizeof(val_s), "v__%d", i);

        try {
            bSucc = n->removeConfig(key_s, NULLSTR);
        }
        catch (NacosException &e) {
            cout <<
                 "Request failed with curl code:" << e.errorcode() << endl <<
                 "Reason:" << e.what() << endl;
            return false;
        }
        cout << "Delete Key:" << key_s << " with value:" << val_s << " result:" << bSucc << endl;
    }

    return true;
}