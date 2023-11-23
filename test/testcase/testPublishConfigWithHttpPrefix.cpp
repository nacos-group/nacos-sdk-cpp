#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "factory/NacosFactoryFactory.h"
#include "ResourceGuard.h"
#include "constant/PropertyKeyConst.h"
#include "src/debug/DebugAssertion.h"
#include "src/log/Logger.h"

using namespace std;
using namespace nacos;

bool testPublishConfigWithHttpPrefix() {
    cout << "in function testPublishConfigWithHttpPrefix" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "htTp://localhost:8848,HtTP://127.0.0.1:8848";
    ADD_AUTH_INFO(props);
    ADD_SPAS_INFO(props);
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    bool bSucc;
    for (int i = 0; i < 50; i++) {
        char key_s[200];
        char val_s[200];
        snprintf(key_s, sizeof(key_s), "Key%d", i);
        snprintf(val_s, sizeof(val_s), "v__%d", i);
        NacosString ss = "";

        try {
            bSucc = n->publishConfig(key_s, NULLSTR, val_s);
            int retry = 0;
            while (!(ss == val_s) && retry++ < 10) {
                sleep(1);
                try {
                    ss = n->getConfig(key_s, NULLSTR, 1000);
                } catch (NacosException &e) { }
            }
            n->removeConfig(key_s, NULLSTR);

            if (!(ss == val_s)) {
                throw NacosException(0, "getConfig() failed.");
            }
        }
        catch (NacosException &e) {
            cout <<
                 "Request failed with curl code:" << e.errorcode() << endl <<
                 "Reason:" << e.what() << endl;

            return false;
        }
        cout << "Publishing Key:" << key_s << " with value:" << val_s << " result:" << bSucc << endl;
    }

    return true;
}