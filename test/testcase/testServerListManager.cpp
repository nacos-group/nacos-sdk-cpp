#include <iostream>
#include <map>
#include "src/server/ServerListManager.h"
#include "src/config/NacosConfigService.h"
#include "factory/NacosServiceFactory.h"
#include "ResourceGuard.h"
#include "src/http/HTTPCli.h"
#include "PropertyKeyConst.h"
#include "src/json/JSON.h"

using namespace std;
using namespace nacos;

bool testServerListManager() {
    cout << "in function testServerListManager" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
    NacosServiceFactory *factory = new NacosServiceFactory(props);
    ResourceGuard <NacosServiceFactory> _guardFactory(factory);
    ConfigService *n = factory->CreateConfigService();
    ResourceGuard <ConfigService> _serviceFactory(n);

    //NacosConfigService *nn = (NacosConfigService *) n;
    //ServerListManager *serverListManager = nn->getServerListManager();

    list <NacosServerInfo> res;// = serverListManager->__debug();

    for (list<NacosServerInfo>::iterator it = res.begin(); it != res.end(); it++) {
        NacosString key = it->getKey();
        NacosString val = it->getCompleteAddress();

        cout << key << ":" << endl << val << endl;
    }

    cout << "=====================cornercase========================" << endl;
    NacosString fakeSvr = "{\"servers\":[]}";
    list <NacosServerInfo> result = JSON::Json2NacosServerInfo(fakeSvr);
    cout << "result.size == " << result.size() << endl;
    for (list<NacosServerInfo>::iterator it = result.begin(); it != result.end(); it++) {
        NacosString key = it->getCompleteAddress();
        NacosString val = it->toString();

        cout << key << ":" << endl << val << endl;
    }
    return true;
}