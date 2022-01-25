#include <iostream>
#include "Nacos.h"

using namespace std;
using namespace nacos;

int main() {
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";//server address
    INacosServiceFactory *factory = NacosFactoryFactory::getNacosFactory(props);
    ResourceGuard <INacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);
    bool bSucc = false;
    NacosString ss = "";

    try {
        bSucc = n->publishConfig("Cfg_key", NULLSTR, "Cfg_val");
        int retry = 0;
        ss = n->getConfig("Cfg_key", NULLSTR, 1000);
        while (!(ss == "Cfg_val") && retry++ < 10) {
            ss = n->getConfig("Cfg_key", NULLSTR, 1000);
        }

        if (!(ss == "Cfg_val")) {
            throw NacosException(0, "getConfig() failed.");
        }
    }
    catch (NacosException &e) {
        cout <<
             "Request failed with curl code:" << e.errorcode() << endl <<
             "Reason:" << e.what() << endl;

        return -1;
    }
    cout << "Publishing Key:Cfg_key with value:Cfg_val result:" << bSucc << endl;

    return 0;
}
