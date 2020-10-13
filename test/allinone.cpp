#include <iostream>
#include <stdio.h>
#include "DebugAssertion.h"
#include "src/init/init.h"

using namespace std;
using namespace nacos;

bool testNormalHttpRequest();

bool testNoServerRequest();

bool testGetConfig();

bool testGetConfigwithDefaultPort();

bool testInvalidConfig();

bool testDebug();

bool testVaArgs();

bool testVaArgs2();

bool testlogPrint();

bool testPublishConfig();

bool testStringEqual();

bool testAddListener();

bool testReadWriteFile();

bool testGetFileSize();

bool testFileExists();

bool testCreateAndRemove();

bool testCleanDirectory();

bool testSaveSnapshot();

bool testCleanTestenvCacheAndGetTestenv();

bool testCleanPrdCacheAndGetPrdenv();

bool testCleanAllCache();

bool testMD5();

bool testURLEncodeAndDecode();

bool testStringExplode();

bool testNamingProxySmokeTest();

bool testNamingServiceRegister();

bool testRapidJsonIntroduce();

bool testSerialize();

bool testThreadSmoke();

bool testThreadPoolSmoke();

bool testString2ServiceInfo();

bool testMalformedJson2ServiceInfo();

bool testMalformedDouble2ServiceInfo();

bool testLackcacheMillisServiceInfo();

bool testGetAllInstances();

bool testListeningKeys();

bool testAppConfigManager();

bool testServerListManager();

bool testDeleteConfig();

bool testEndpointWithNamingProxy();

bool testUUID();

bool testUUIDMT();

bool testListenService();

bool testGetServiceNames();

bool testInstanceSelectors();

bool testRandomByWeightSelector();

bool testThreadLocal();
bool testThreadLocalPtr();
bool testThreadLocalPtrWithInitializer();

TestData testList1[] =
TEST_ITEM_START

TEST_ITEM_END

TestData
testList[] =
TEST_ITEM_START
        TEST_ITEM("Listen to key and remove it from listening list", testListeningKeys)
TEST_ITEM_END

int main() {
    cout << "Please start a nacos server listening on port 8848 in this machine first." << endl;
    cout << "And when the server is ready, press any key to start the test." << endl;
    getchar();
    Init::doInit();
    for (size_t i = 0; i < sizeof(testList) / sizeof(TestData); i++) {
        cout << "===========================" << endl;
        TestData * curtest = &testList[i];
        TESTFN testfunction = curtest->testFn;
        cout << "Testing " << curtest->testName << " ..." << endl;
        bool pass = testfunction();
        if (!pass) {
            cout << "FAILED" << endl;
        } else {
            cout << "PASSED!" << endl;
        }
    }
    cout << "===========================" << endl;

    Init::doDeinit();
    return 0;
}