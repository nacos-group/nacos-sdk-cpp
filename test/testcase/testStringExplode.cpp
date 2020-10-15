#include <iostream>
#include <stdlib.h>
#include <list>
#include "utils/ParamUtils.h"
#include "DebugAssertion.h"
#include "Debug.h"

using namespace std;
using namespace nacos;

bool testStringExplode() {
    cout << "in function testStringExplode" << endl;

    vector <NacosString> explodedList;
    NacosString originalContent = "Hello|World|My|Name|Is";
    ParamUtils::Explode(explodedList, originalContent, '|');
    SHOULD_BE_TRUE(explodedList.size() == 5,
                   "Exploding Hello|World|My|Name|Is with separator | should get a list with size 5.");
    SHOULD_BE_TRUE(explodedList[0] == "Hello", "explodedList[0] should be Hello");
    SHOULD_BE_TRUE(explodedList[1] == "World", "explodedList[1] should be World");
    SHOULD_BE_TRUE(explodedList[2] == "My", "explodedList[2] should be My");
    SHOULD_BE_TRUE(explodedList[3] == "Name", "explodedList[3] should be Name");
    SHOULD_BE_TRUE(explodedList[4] == "Is", "explodedList[4] should be Is");

    vector <NacosString> specialExplode;
    NacosString specialoriginalContent = "Hello\x1World\x1My\x1Name\x1Is";
    ParamUtils::Explode(specialExplode, specialoriginalContent, '\x1');
    SHOULD_BE_TRUE(specialExplode.size() == 5,
                   "Exploding Hello\x1World\x1My\x1Name\x1Is with separator \x1 should get a list with size 5.");
    SHOULD_BE_TRUE(specialExplode[0] == "Hello", "specialExplode[0] should be Hello");
    SHOULD_BE_TRUE(specialExplode[1] == "World", "specialExplode[1] should be World");
    SHOULD_BE_TRUE(specialExplode[2] == "My", "specialExplode[2] should be My");
    SHOULD_BE_TRUE(specialExplode[3] == "Name", "specialExplode[3] should be Name");
    SHOULD_BE_TRUE(specialExplode[4] == "Is", "specialExplode[4] should be Is");

    vector <NacosString> nullEnd;
    NacosString nullEndStr = "k=";
    ParamUtils::Explode(nullEnd, nullEndStr, '=');
    SHOULD_BE_TRUE(nullEnd.size() == 2, "exploding k= should get 2 items");
    SHOULD_BE_TRUE(nullEnd[0] == "k", "nullEnd[0] should be k");
    SHOULD_BE_TRUE(nullEnd[1] == "", "nullEnd[0] should be empty");

    return true;
}
