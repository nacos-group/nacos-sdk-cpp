#include <iostream>
#include "utils//UuidUtils.h"
#include "thread/Thread.h"
#include "thread/ThreadLocal.h"
#include "DebugAssertion.h"
#include "utils/RandomUtils.h"

using namespace std;

ThreadLocal<NacosString> threadLocal;
ThreadLocal<int*> threadLocalPtr;
ThreadLocal<int*> threadLocalPtrWithInitializer;

class ThreadLocalWithInit : public ThreadLocal<int*> {
public:
    void onCreate(int **value){
        *value = (int*)0xffff;
    }

    void onDestroy(int **value) {
        log_debug("ThreadLocalWithInit::onDestroy is called, ptr value: %p", *value);
    }
};

void *ThreadLocalFuncs4Ptr(void *param) {
    log_debug("threadLocalPtr.get() : %p, should be null", threadLocalPtr.get());
    NACOS_ASSERT(threadLocalPtr.get() == NULL);

    for (int i = 0; i < 100; i++) {
        int* rndPtr = reinterpret_cast<int*>(RandomUtils::random(0, 1000));
        threadLocalPtr.set(rndPtr);

        NACOS_ASSERT(rndPtr == threadLocalPtr.get())
    }

    return NULL;
}

void *ThreadLocalFuncs4PtrWithInitializer(void *param) {
    log_debug("threadLocalPtr.get() : %p, should be null", threadLocalPtr.get());
    NACOS_ASSERT(threadLocalPtrWithInitializer.get() == (int*)0xFFFF);

    for (int i = 0; i < 100; i++) {
        int* rndPtr = reinterpret_cast<int*>(RandomUtils::random(0, 1000));
        threadLocalPtrWithInitializer.set(rndPtr);

        NACOS_ASSERT(rndPtr == threadLocalPtrWithInitializer.get())
    }

    return NULL;
}

void *ThreadLocalFuncs(void *param) {
    Thread *thisThread = *((Thread **) param);

    for (int i = 0; i < 10; i++) {
        threadLocal.set(UuidUtils::generateUuid().c_str());
        log_debug("Thread %s UUID: %s\n", thisThread->getThreadName().c_str(), threadLocal.get().c_str());
    }

    return NULL;
}

bool testThreadLocal() {
    cout << "in function testThreadLocal" << endl;

    cout << "Generating threads..." << endl;

    Thread *threads[10] = {NULL};
    for (int i = 0; i < 10; i++) {
        NacosString threadName = "Thread-" + NacosStringOps::valueOf(i);
        threads[i] = new Thread(threadName, ThreadLocalFuncs, (void *) &threads[i]);
        threads[i]->start();
    }

    for (int i = 0; i < 10; i++) {
        threads[i]->join();
    }

    cout << "test end..." << endl;

    return true;
}

bool testThreadLocalPtr() {
    cout << "in function testThreadLocalPtr" << endl;

    cout << "Generating threads..." << endl;

    Thread *threads[10] = {NULL};
    for (int i = 0; i < 10; i++) {
        NacosString threadName = "ThreadPtr-" + NacosStringOps::valueOf(i);
        threads[i] = new Thread(threadName, ThreadLocalFuncs4Ptr, (void *) &threads[i]);
        threads[i]->start();
    }

    for (int i = 0; i < 10; i++) {
        threads[i]->join();
    }

    cout << "test end..." << endl;

    return true;
}

bool testThreadLocalPtrWithInitializer() {
    cout << "in function testThreadLocalPtrWithInitializer" << endl;

    cout << "Generating threads..." << endl;

    Thread *threads[10] = {NULL};
    for (int i = 0; i < 10; i++) {
        NacosString threadName = "ThreadPtr-" + NacosStringOps::valueOf(i);
        threads[i] = new Thread(threadName, ThreadLocalFuncs4PtrWithInitializer, (void *) &threads[i]);
        threads[i]->start();
    }

    for (int i = 0; i < 10; i++) {
        threads[i]->join();
    }

    cout << "test end..." << endl;

    return true;
}