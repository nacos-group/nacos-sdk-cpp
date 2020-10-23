#include <iostream>
#include <stdlib.h>
#include "src/config/LocalConfigInfoProcessor.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"
#include "ResourceGuard.h"

using namespace std;
using namespace nacos;

bool testSaveSnapshot() {
    cout << "in function testSaveSnapshot" << endl;
    Properties props;
    AppConfigManager *appConfigManager = new AppConfigManager(props);
    LocalConfigInfoProcessor *localConfigInfoProcessor = new LocalConfigInfoProcessor(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalConfigInfoProcessor> __guardCfgProcessor(localConfigInfoProcessor);

    localConfigInfoProcessor->cleanAllSnapshot();
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "ConfigName=Value for FrontTenant");
    NacosString content = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    SHOULD_BE_TRUE(content == "ConfigName=Value for FrontTenant", "Saved snapshot, read it again, should be the same");
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "ConfigName=Value");
    content = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    SHOULD_BE_TRUE(content == "ConfigName=Value", "Saved snapshot(No tenant), read it again, should be the same");
    return true;
}

bool testCleanTestenvCacheAndGetTestenv() {
    cout << "in function testCleanTestenvCacheAndGetTestenv" << endl;
    Properties props;
    AppConfigManager *appConfigManager = new AppConfigManager(props);
    LocalConfigInfoProcessor *localConfigInfoProcessor = new LocalConfigInfoProcessor(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalConfigInfoProcessor> __guardCfgProcessor(localConfigInfoProcessor);
    localConfigInfoProcessor->cleanAllSnapshot();
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    localConfigInfoProcessor->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    localConfigInfoProcessor->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    localConfigInfoProcessor->cleanEnvSnapshot("Testenv");
    cntfrontEndTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "", "Testenv removed, Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv",
                   "Testenv removed, Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "", "Testenv removed, Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Testenv removed, Check settings for Prodenv");
    return true;
}

bool testCleanPrdCacheAndGetPrdenv() {
    cout << "in function testCleanPrdCacheAndGetPrdenv" << endl;
    Properties props;
    AppConfigManager *appConfigManager = new AppConfigManager(props);
    LocalConfigInfoProcessor *localConfigInfoProcessor = new LocalConfigInfoProcessor(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalConfigInfoProcessor> __guardCfgProcessor(localConfigInfoProcessor);
    localConfigInfoProcessor->cleanAllSnapshot();
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    localConfigInfoProcessor->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    localConfigInfoProcessor->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    localConfigInfoProcessor->cleanEnvSnapshot("Prodenv");
    cntfrontEndTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv",
                   "Prodenv removed, Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "", "Prodenv removed, Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Prodenv removed, Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "", "Prodenv removed, Check settings for Prodenv");
    return true;
}

bool testCleanAllCache() {
    cout << "in function testCleanAllCache" << endl;
    Properties props;
    AppConfigManager *appConfigManager = new AppConfigManager(props);
    LocalConfigInfoProcessor *localConfigInfoProcessor = new LocalConfigInfoProcessor(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalConfigInfoProcessor> __guardCfgProcessor(localConfigInfoProcessor);
    localConfigInfoProcessor->cleanAllSnapshot();
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    localConfigInfoProcessor->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    localConfigInfoProcessor->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    localConfigInfoProcessor->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    localConfigInfoProcessor->cleanAllSnapshot();
    cntfrontEndTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = localConfigInfoProcessor->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = localConfigInfoProcessor->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "", "cleanAllSnapshot(), Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "", "cleanAllSnapshot(), Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "", "cleanAllSnapshot(), Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "", "cleanAllSnapshot(), Check settings for Prodenv");
    return true;
}