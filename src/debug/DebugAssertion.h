#ifndef __DEBUG_ASSERTION_H_
#define __DEBUG_ASSERTION_H_

#include <stdlib.h>
#include <stdio.h>
#include "NacosString.h"

namespace nacos{
#define TEST_ITEM_START {
#define TEST_ITEM(testName, testfn) {(testName), (testfn)},
#define TEST_ITEM_END };

#define NACOS_ASSERT(x) \
if (!(x)) \
{ \
    printf("Assertion failed! file:" __FILE__":%d\n", __LINE__); \
    abort(); \
}

typedef bool (*TESTFN)();

typedef struct tagTestData {
    NacosString testName;
    TESTFN testFn;
} TestData;

#define SHOULD_BE_TRUE(assertion, message) \
do \
{ \
    if (!(assertion)) \
    { \
        cout << (message) << "...:failed" << endl; \
        return false; \
    } \
    cout << (message) << "...:passed" << endl; \
} while(0);

#define SHOULD_BE_FALSE(assertion, message) SHOULD_BE_TRUE(!(assertion), (message))

#ifdef NACOS_AUTH
#define ADD_AUTH_INFO(x) \
do { \
    (x)["nacos.auth.username"] = "nacos"; \
    (x)["nacos.auth.password"] = "nacos"; \
} while (0)
#else
#define ADD_AUTH_INFO(x)
#endif

#ifdef NACOS_SPAS
#define ADD_SPAS_INFO(x) \
do { \
    (x)["secretKey"] = "nacos"; \
    (x)["accessKey"] = "nacos"; \
} while (0)
#else
#define ADD_SPAS_INFO(x)
#endif

}//namespace nacos
#endif