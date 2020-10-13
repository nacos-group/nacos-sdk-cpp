#include <iostream>
#include <stdlib.h>
#include "src/md5/md5.h"
#include "DebugAssertion.h"

using namespace std;
using namespace nacos;

bool testMD5() {
    cout << "in function testMD5" << endl;
    MD5 md5;

    cout << "Checking md5 for \"a\"" << endl;
    md5.reset();
    md5.update("a");
    cout << "md5:" << md5.toString() << endl;
    SHOULD_BE_TRUE(md5.toString() == "0cc175b9c0f1b6a831c399e269772661",
                   "NacosString \"a\" md5 should be 0cc175b9c0f1b6a831c399e269772661");
    cout << "Checking md5 for \"aaaaaaa\"" << endl;
    md5.reset();
    md5.update("aaaaaaa");
    cout << "md5:" << md5.toString() << endl;
    SHOULD_BE_TRUE(md5.toString() == "5d793fc5b00a2348c3fb9ab59e5ca98a",
                   "NacosString \"aaaaaaa\" md5 should be 5d793fc5b00a2348c3fb9ab59e5ca98a");
    cout << "Checking md5 for \"2652451346435432\"" << endl;
    md5.reset();
    md5.update("2652451346435432");
    cout << "md5:" << md5.toString() << endl;
    SHOULD_BE_TRUE(md5.toString() == "69a8b79497dc611dcf9326a0b232ae55",
                   "NacosString \"2652451346435432\" md5 should be 69a8b79497dc611dcf9326a0b232ae55");
    return true;
}
