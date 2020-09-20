#include <iostream>
#include "factory/NacosServiceFactory.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "ResourceGuard.h"

using namespace std;

bool testDeleteConfig() {
    cout << "in function testDeleteConfig" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    bool bSucc;
    for (int i = 5; i < 50; i++) {
        char key_s[200];
        char val_s[200];
        sprintf(key_s, "Key%d", i);
        sprintf(val_s, "v__%d", i);

        try {
            bSucc = n->removeConfig(key_s, NULLSTR);
        }
        catch (NacosException e) {
            cout <<
                 "Request failed with curl code:" << e.errorcode() << endl <<
                 "Reason:" << e.what() << endl;
            return false;
        }
        cout << "Delete Key:" << key_s << " with value:" << val_s << " result:" << bSucc << endl;
    }

    return true;
}