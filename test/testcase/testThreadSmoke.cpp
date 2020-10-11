#include <iostream>
#include "DebugAssertion.h"
#include "Debug.h"
#include "src/thread/Thread.h"
#include "src/thread/ThreadPool.h"
#include "NacosString.h"

using namespace std;
using namespace nacos;

class SmokingTestThreadTask : public Task {
public:
    SmokingTestThreadTask(const NacosString &taskName) { setTaskName(taskName); };

    void run() {
        NacosString taskName = getTaskName();
        log_info("Hello world from %s\n", taskName.c_str());
    };
};

class ThreadPoolLongRunTask : public Task {
public:
    ThreadPoolLongRunTask(int taskId) {
        NacosString taskName = "ThreadPoolLongRunTask" + NacosStringOps::valueOf(taskId);
        setTaskName(taskName);
    };

    void run() {
        NacosString taskName = getTaskName();
        log_info("Hello world from %s\n", taskName.c_str());
        log_info("Run for 10 secs\n");
        sleep(5);
        log_info("ok\n");
    };
};

void *threadFunc(void *param) {
    log_info("Hello from threadFunc() in another thread, stopping threadpool\n");
    return NULL;
}

bool testThreadSmoke() {
    cout << "In testThreadSmoke" << endl;
    Thread t("testThread", threadFunc);
    t.start();
    t.join();

    cout << "If no coredump, should be successful" << endl;

    return true;
}

void *threadStopper(void *param) {
    ThreadPool *tp = (ThreadPool *) param;
    log_info("Hello from threadStopper() in another thread, stopping threadpool\n");
    sleep(10);
    tp->stop();
    log_info("threadStopper():ok\n");
    return NULL;
}

bool testThreadPoolSmoke() {
    cout << "Starting testThreadPoolSmoke..." << endl;
    ThreadPool tp(10);
    tp.start();
    cout << "ok, size = 32" << endl;

    Thread tpStopper("tpStopper", threadStopper, &tp);
    tpStopper.start();

    Task *tasks[1000];
    for (size_t i = 0; i < 40; i++) {
        cout << "Creating task " << i << "..." << endl;
        tasks[i] = new ThreadPoolLongRunTask(i);
        tp.put(tasks[i]);
        cout << "ok" << endl;
    }

    tp.stop();
    for (size_t i = 0; i < 40; i++) {
        cout << "destroying task " << i << "..." << endl;
        delete tasks[i];
        tasks[i] = NULL;
        cout << "ok" << endl;
    }
    cout << "If no coredump, should be successful" << endl;
    tpStopper.join();
    return true;
}
