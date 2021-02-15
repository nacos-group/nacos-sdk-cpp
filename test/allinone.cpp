#include <iostream>
#include <stdio.h>
#include "DebugAssertion.h"
#include "src/init/Init.h"
#include "src/log/Logger.h"
#include <list>

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

bool testStringExplode2();

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

bool testMaintainGetService();

bool testMaintainUpdateService();

bool testMaintainCreateService();

bool testMaintainUpdateInstance();

bool testPublishConfigWithHttpPrefix();

bool testRemoveKeyBeingWatched();

bool testGetHostIp();

bool testDelayedThread();

TestData disabledTestList[] =
TEST_ITEM_START
TEST_ITEM_END

TestData
testList[] =
TEST_ITEM_START

    TEST_ITEM("Test delayed task pool", testNamingServiceRegister)
TEST_ITEM_END

int main() {
    Init::doInit();
    list<TestData *> failed_list;
    cout << "Please start a nacos server listening on port 8848 in this machine first." << endl;
    cout << "And when the server is ready, press any key to start the test." << endl;
    getchar();
    int nr_succ = 0, nr_fail = 0;
    Logger::setLogLevel(DEBUG);
    cout << "BEGIN OF TESTS" << endl;
    cout << "===========================" << endl;
    for (size_t i = 0; i < sizeof(testList) / sizeof(TestData); i++) {
        TestData * curtest = &testList[i];
        TESTFN testfunction = curtest->testFn;
        cout << "Testing " << curtest->testName << " ..." << endl;
        bool pass = testfunction();
        if (!pass) {
            cout << "FAILED" << endl;
            failed_list.push_back(curtest);
            nr_fail++;
        } else {
            cout << "PASSED!" << endl;
            nr_succ++;
        }
        cout << "===========================" << endl;
    }

    if (!failed_list.empty()) {
        cout << "List of failed cases:" << endl;
        for (list<TestData*>::iterator it = failed_list.begin(); it != failed_list.end(); it++) {
            cout << (*it)->testName << endl;
        }
        cout << "===========================" << endl;
    }

    cout << "SUMMARY" << endl;
    cout << "Total:" << nr_succ + nr_fail << endl;
    cout << "Succ:" << nr_succ << endl;
    cout << "Fail:" << nr_fail << endl;
    cout << "===========================" << endl;
    return 0;
}