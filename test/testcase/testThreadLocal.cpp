#include <iostream>
#include "utils//UuidUtils.h"
#include "thread/Thread.h"
#include "thread/ThreadLocal.h"

using namespace std;

ThreadLocal<NacosString> threadLocal;

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
