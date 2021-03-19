#include <iostream>
#include <unistd.h>
#include "DebugAssertion.h"
#include "src/log/Logger.h"
#include "src/thread/Thread.h"
#include "src/thread/ThreadPool.h"
#include "NacosString.h"
#include "thread/AtomicInt.h"

using namespace std;
using namespace nacos;

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

AtomicInt totalFinishedThreads;

class SmokingTestThreadTask : public Task {
private:
    AtomicInt &_counter;
public:
    SmokingTestThreadTask(const NacosString &taskName, AtomicInt &counter) : _counter(counter) {
        setTaskName(taskName);
    };

    void run() {
        NacosString taskName = getTaskName();
        int currentTotal = 0;
        for (int i = 0; i < 100000; i++) {
            currentTotal = _counter.inc();
        }
        log_info("Hello world from %s, totalCount = %d\n", taskName.c_str(), currentTotal);
        totalFinishedThreads.inc();
    };
};

bool testThreadPoolConcurrentWithAtomicCounter() {
    cout << "Starting testThreadPoolConcurrentWithAtomicCounter..." << endl;
    ThreadPool tp(10);
    tp.start();
    cout << "ok, size = 10" << endl;
    AtomicInt totalCounter;

    Task *tasks[1000];
    for (size_t i = 0; i < 40; i++) {
        cout << "Creating task " << i << "..." << endl;
        tasks[i] = new SmokingTestThreadTask("Counter-" + NacosStringOps::valueOf(i), totalCounter);
        tp.put(tasks[i]);
        cout << "ok" << endl;
    }


    while (totalFinishedThreads.get() != 40) {
        sleep(1);
    }
    tp.stop();
    cout << "If no coredump, should be successful" << endl;
    cout << "totalCounter.get() = " << totalCounter.get() << endl;

    SHOULD_BE_TRUE(totalCounter.get() == 40 * 100000, "40 threads, each thread accumulates totalCounter by 100k, result should be 4000k");

    for (size_t i = 0; i < 40; i++) {
        cout << "destroying task " << i << "..." << endl;
        delete tasks[i];
        tasks[i] = NULL;
        cout << "ok" << endl;
    }
    return true;
}