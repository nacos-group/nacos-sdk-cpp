#include <iostream>
#include <stdlib.h>
#include "config/LocalConfigInfoProcessor.h"
#include "DebugAssertion.h"
#include "Debug.h"
#include "NacosString.h"

using namespace std;

bool testSaveSnapshot() {
    cout << "in function testSaveSnapshot" << endl;
    LocalConfigInfoProcessor::cleanAllSnapshot();
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "ConfigName=Value for FrontTenant");
    NacosString content = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    SHOULD_BE_TRUE(content == "ConfigName=Value for FrontTenant", "Saved snapshot, read it again, should be the same");
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "ConfigName=Value");
    content = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    SHOULD_BE_TRUE(content == "ConfigName=Value", "Saved snapshot(No tenant), read it again, should be the same");
    return true;
}

bool testCleanTestenvCacheAndGetTestenv() {
    cout << "in function testCleanTestenvCacheAndGetTestenv" << endl;
    LocalConfigInfoProcessor::cleanAllSnapshot();
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    LocalConfigInfoProcessor::saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    LocalConfigInfoProcessor::saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    LocalConfigInfoProcessor::cleanEnvSnapshot("Testenv");
    cntfrontEndTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "", "Testenv removed, Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv",
                   "Testenv removed, Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "", "Testenv removed, Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Testenv removed, Check settings for Prodenv");
    return true;
}

bool testCleanPrdCacheAndGetPrdenv() {
    cout << "in function testCleanPrdCacheAndGetPrdenv" << endl;
    LocalConfigInfoProcessor::cleanAllSnapshot();
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    LocalConfigInfoProcessor::saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    LocalConfigInfoProcessor::saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    LocalConfigInfoProcessor::cleanEnvSnapshot("Prodenv");
    cntfrontEndTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv",
                   "Prodenv removed, Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "", "Prodenv removed, Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Prodenv removed, Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "", "Prodenv removed, Check settings for Prodenv");
    return true;
}

bool testCleanAllCache() {
    cout << "in function testCleanAllCache" << endl;
    LocalConfigInfoProcessor::cleanAllSnapshot();
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Testenv");
    LocalConfigInfoProcessor::saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant",
                                           "Value for FrontTenant&Prodenv");
    LocalConfigInfoProcessor::saveSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Testenv");
    LocalConfigInfoProcessor::saveSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR, "Value for Prodenv");

    NacosString cntfrontEndTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1",
                                                                        "FrontTenant");
    NacosString cntPrdFrontEnd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1",
                                                                       "FrontTenant");
    NacosString cntTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    NacosString cntPrd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "Value for FrontTenant&Testenv", "Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "Value for FrontTenant&Prodenv", "Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "Value for Testenv", "Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "Value for Prodenv", "Check settings for Prodenv");

    LocalConfigInfoProcessor::cleanAllSnapshot();
    cntfrontEndTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntPrdFrontEnd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", "FrontTenant");
    cntTest = LocalConfigInfoProcessor::getSnapshot("Testenv", "DummyData", "BusinessGrp1", NULLSTR);
    cntPrd = LocalConfigInfoProcessor::getSnapshot("Prodenv", "DummyData", "BusinessGrp1", NULLSTR);

    SHOULD_BE_TRUE(cntfrontEndTest == "", "cleanAllSnapshot(), Check settings for FrontTenant&Testenv");
    SHOULD_BE_TRUE(cntPrdFrontEnd == "", "cleanAllSnapshot(), Check settings for FrontTenant&Prodenv");
    SHOULD_BE_TRUE(cntTest == "", "cleanAllSnapshot(), Check settings for Testenv");
    SHOULD_BE_TRUE(cntPrd == "", "cleanAllSnapshot(), Check settings for Prodenv");
    return true;
}