#include <iostream>
#include "DebugAssertion.h"
#include "src/config/IOUtils.h"
#include "utils/DirUtils.h"
#include "Debug.h"

#define ENCODING "UTF-8"

using namespace std;
using namespace nacos;

bool testReadWriteFile() {
    cout << "in function testReadWriteFile" << endl;

    NacosString cwd = DirUtils::getCwd();
    NacosString tmpPath = cwd + "/tmp";
    IOUtils::recursivelyCreate(tmpPath);
    NacosString writeFile = tmpPath + "/testfile";
    IOUtils::writeStringToFile(writeFile, "nacos-cpp-cli-test", ENCODING);

    NacosString content = IOUtils::readStringFromFile(writeFile, ENCODING);
    SHOULD_BE_TRUE(content == "nacos-cpp-cli-test",
                   "NacosString \"nacos-cpp-cli-test\" is written to file, should be the same when reading");
    return true;
}

bool testGetFileSize() {
    cout << "in function testGetFileSize" << endl;

    NacosString cwd = DirUtils::getCwd();
    NacosString tmpPath = cwd + "/tmp";
    IOUtils::recursivelyCreate(tmpPath);
    NacosString writeFile = tmpPath + "/testfile";
    IOUtils::writeStringToFile(writeFile, "nacos-cpp-cli-test", ENCODING);

    size_t sz = IOUtils::getFileSize(writeFile);
    SHOULD_BE_TRUE(sz == 18, "NacosString \"nacos-cpp-cli-test\" is written to file, size should be 18");
    return true;
}

bool testFileExists() {
    cout << "in function testFileExists" << endl;

    NacosString cwd = DirUtils::getCwd();
    NacosString tmpPath = cwd + "/tmp";
    NacosString writeFile = tmpPath + "/testfile";
    IOUtils::recursivelyRemove(tmpPath);

    cout << "Cwd:" << cwd << endl;
    SHOULD_BE_TRUE(IOUtils::checkNotExistOrNotFile(cwd), "The source folder should be a folder, not a file");
    SHOULD_BE_FALSE(IOUtils::checkNotExistOrNotDir(cwd), "The source folder should be a folder");

    IOUtils::recursivelyCreate(tmpPath);
    IOUtils::writeStringToFile(writeFile, "nacos-cpp-cli-test", ENCODING);
    SHOULD_BE_FALSE(IOUtils::checkNotExistOrNotFile(writeFile),
                    "Created a file named \"testfile\" with content \"nacos-cpp-cli-test\"");
    return true;
}


bool testCreateAndRemove() {
    cout << "in function testFileExists" << endl;

    NacosString cwd = DirUtils::getCwd();
    NacosString tmpPath = cwd + "/tmp/123/456/789/2312/afda/4__dsa/dd_";
    NacosString rmpath = cwd + "/tmp/123";
    IOUtils::recursivelyCreate(tmpPath);
    SHOULD_BE_FALSE(IOUtils::checkNotExistOrNotDir(tmpPath), "We have created a dir, it should exist");
    IOUtils::recursivelyRemove(rmpath);
    SHOULD_BE_TRUE(IOUtils::checkNotExistOrNotFile(cwd + "/tmp"), "Removed everything except the tmp folder");
    NacosString easterEgg = cwd + "/tmp/Liao/Sijia";
    IOUtils::recursivelyCreate(easterEgg);
    return true;
}

bool testCleanDirectory() {
    cout << "in function testCleanDirectory" << endl;

    NacosString cwd = DirUtils::getCwd();
    NacosString tmpPath = cwd + "/tmp/testcleandir/456/789/2312/afda/4__dsa/dd_";
    NacosString cleanPath = cwd + "/tmp/testcleandir";
    IOUtils::recursivelyCreate(tmpPath);
    SHOULD_BE_FALSE(IOUtils::checkNotExistOrNotDir(tmpPath), "We have created a dir, it should exist");
    IOUtils::cleanDirectory(cleanPath);
    SHOULD_BE_TRUE(IOUtils::checkNotExistOrNotDir(tmpPath),
                   "Subdirectories of the dir is cleaned, it should not exist");
    SHOULD_BE_FALSE(IOUtils::checkNotExistOrNotDir(cleanPath), "The dir is cleaned, but itself should exist");
    return true;
}