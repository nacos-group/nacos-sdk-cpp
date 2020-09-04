#include <iostream>
#include <map>
#include "server/ServerListManager.h"
#include "http/HTTPCli.h"
#include "PropertyKeyConst.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "json/JSON.h"
#include "Constants.h"

using namespace std;

bool testServerListManager()
{
	cout << "in function testServerListManager" << endl;
    Properties props;
    props[PropertyKeyConst::SERVER_ADDR] = "127.0.0.1:8848";
	ServerListManager serverListManager(props);
    HTTPCli *httpCli = new HTTPCli();
    serverListManager.setHttpCli(httpCli);
    std::map<NacosString, NacosServerInfo> res = serverListManager.__debug();

    for (std::map<NacosString, NacosServerInfo>::iterator it = res.begin(); it != res.end(); it++)
    {
        NacosString key = it->first;
        NacosString val = it->second.toString();

        cout << key << ":" << endl << val << endl;
    }

    cout << "=====================cornercase========================" << endl;
    NacosString fakeSvr = "{\"servers\":[]}";
    map<NacosString, NacosServerInfo> result = JSON::Json2NacosServerInfo(fakeSvr);
    cout << "result.size == " << result.size() << endl;
    for (std::map<NacosString, NacosServerInfo>::iterator it = result.begin(); it != result.end(); it++)
    {
        NacosString key = it->first;
        NacosString val = it->second.toString();

        cout << key << ":" << endl << val << endl;
    }
	return true;
}