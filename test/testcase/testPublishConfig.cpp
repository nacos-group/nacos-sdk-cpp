#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;
using namespace nacos;

bool testPublishConfig() {
    cout << "in function testPublishConfig" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    bool bSucc;
    for (int i = 0; i < 50; i++) {
        char key_s[200];
        char val_s[200];
        sprintf(key_s, "Key%d", i);
        sprintf(val_s, "v__%d", i);
        NacosString ss = "";

        try {
            bSucc = n->publishConfig(key_s, NULLSTR, val_s);
            int retry = 0;
            ss = n->getConfig(key_s, NULLSTR, 1000);
            while (!(ss == val_s) && retry++ < 10) {
                ss = n->getConfig(key_s, NULLSTR, 1000);
            }

            if (!(ss == val_s)) {
                throw NacosException(0, "getConfig() failed.");
            }
        }
        catch (NacosException e) {
            cout <<
                 "Request failed with curl code:" << e.errorcode() << endl <<
                 "Reason:" << e.what() << endl;

            return false;
        }
        cout << "Publishing Key:" << key_s << " with value:" << val_s << " result:" << bSucc << endl;
    }

    return true;
}