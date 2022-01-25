#include <iostream>
#include "src/utils//NetUtils.h"
#include "src/debug/DebugAssertion.h"

using namespace std;
using namespace nacos;

bool testGetHostIp() {
    cout << "in function testGetHostIp" << endl;

    NacosString ip = NetUtils::getHostIp();
    cout << "ip:" << ip << endl;
    SHOULD_BE_TRUE(!ip.empty(), "Ip got for local machine should not be empty");
    cout << "test end..." << endl;

    return true;
}