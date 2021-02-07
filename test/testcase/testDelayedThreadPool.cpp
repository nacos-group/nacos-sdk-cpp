#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "src/thread/DelayedThreadPool.h"
#include "DebugAssertion.h"
#include "NacosExceptions.h"

using namespace std;
using namespace nacos;

class DelayedTask : public Task {
public:
    DelayedThreadPool *executor;
    uint64_t interval;// in MS
    void run() {
        uint64_t now_ms = TimeUtils::getCurrentTimeInMs();
        executor->schedule(this,now_ms + interval);//interval/1000 secs later
        if (executor == NULL) {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM, "no executor");
        }
        cout << ">>>>>>>>>>>>>>>>>>>>Task " << getTaskName().c_str() <<" triggered, time = " << now_ms/1000 << "(" << now_ms << ")"<< endl;

        sleep(1);
    }
};

bool testDelayedThread() {
    cout << "in function testDelayedThread" << endl;

    DelayedThreadPool dtp("testDPool", 2);
    dtp.start();
    cout << "create tasks" << endl;
    DelayedTask task1;
    task1.executor = &dtp;
    task1.setTaskName("task1");
    task1.interval = 1000;
    DelayedTask task2;
    task2.executor = &dtp;
    task2.setTaskName("task2");
    task2.interval = 2000;
    DelayedTask task3;
    task3.executor = &dtp;
    task3.setTaskName("task3");
    task3.interval = 3000;

    cout << "do schedule" << endl;
    uint64_t now_ms = TimeUtils::getCurrentTimeInMs();
    dtp.schedule(&task1, now_ms + task1.interval);
    dtp.schedule(&task2, now_ms + task2.interval);
    dtp.schedule(&task3, now_ms + task3.interval);
    sleep(6);
    cout << "call stop()" << endl;
    dtp.stop();
    cout << "end of test" << endl;

    return true;
}
