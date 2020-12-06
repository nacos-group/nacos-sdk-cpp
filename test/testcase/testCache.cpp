#include <iostream>
#include "src/config/LocalSnapshotManager.h"
#include "DebugAssertion.h"
#include "NacosString.h"
#include "ResourceGuard.h"

using namespace std;
using namespace nacos;

bool testSaveSnapshot() {
    cout << "in function testSaveSnapshot" << endl;
    Properties props;
    AppConfigManager *appConfigManager = new AppConfigManager(props);
    LocalSnapshotManager *localSnapshotManager = new LocalSnapshotManager(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalSnapshotManager> __guardCfgProcessor(localSnapshotManager);

    localSnapshotManager->cleanAllSnapshot();
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "ConfigName=Value for FrontTenant");
    NacosString content = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    SHOULD_BE_TRUE(content == "ConfigName=Value for FrontTenant", "Saved snapshot, read it again, should be the same");
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "ConfigName=Value");
    content = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    SHOULD_BE_TRUE(content == "ConfigName=Value", "Saved snapshot(No tenant), read it again, should be the same");
    return true;
}

bool testCleanTestenvCacheAndGetTestenv() {
    cout << "in function testCleanTestenvCacheAndGetTestenv" << endl;
    Properties props;
    AppConfigManager *appConfigManager = new AppConfigManager(props);
    LocalSnapshotManager *localSnapshotManager = new LocalSnapshotManager(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalSnapshotManager> __guardCfgProcessor(localSnapshotManager);
    localSnapshotManager->cleanAllSnapshot();
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    localSnapshotManager->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    localSnapshotManager->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    localSnapshotManager->cleanEnvSnapshot("Testenv");
    cntfrontEndTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

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
    LocalSnapshotManager *localSnapshotManager = new LocalSnapshotManager(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalSnapshotManager> __guardCfgProcessor(localSnapshotManager);
    localSnapshotManager->cleanAllSnapshot();
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    localSnapshotManager->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    localSnapshotManager->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    localSnapshotManager->cleanEnvSnapshot("Prodenv");
    cntfrontEndTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

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
    LocalSnapshotManager *localSnapshotManager = new LocalSnapshotManager(appConfigManager);
    ResourceGuard<AppConfigManager> __guardCfg(appConfigManager);
    ResourceGuard<LocalSnapshotManager> __guardCfgProcessor(localSnapshotManager);
    localSnapshotManager->cleanAllSnapshot();
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    localSnapshotManager->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    localSnapshotManager->saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    localSnapshotManager->saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    localSnapshotManager->cleanAllSnapshot();
    cntfrontEndTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = localSnapshotManager->getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = localSnapshotManager->getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "", "cleanAllSnapshot(), Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "", "cleanAllSnapshot(), Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "", "cleanAllSnapshot(), Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "", "cleanAllSnapshot(), Check settings for Prodenv");
    return true;
}