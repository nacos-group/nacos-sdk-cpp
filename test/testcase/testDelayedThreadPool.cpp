#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "src/thread/DelayedThreadPool.h"
#include "DebugAssertion.h"
#include "NacosExceptions.h"

using namespace std;
using namespace nacos;

class DelayedTask : public Task {
public:
    DelayedThreadPool *executor;
    uint64_t interval;// in MS
    uint64_t last_exec_time;
    DelayedTask() {
        last_exec_time = 0;
    }

    void run() {
        uint64_t now_ms = TimeUtils::getCurrentTimeInMs();
        uint64_t interval_calc = 0;
        if (last_exec_time != 0) {
            interval_calc = now_ms - last_exec_time;
        }
        last_exec_time = now_ms;
        executor->schedule(this,now_ms + interval);//interval/1000 secs later
        if (executor == NULL) {
            throw NacosException(NacosException::INVALID_CONFIG_PARAM, "no executor");
        }
        printf(">>>>>>>>>>>>>>>>>>Task %s triggered, time =%llu (%llu), interval = %llu\n", getTaskName().c_str(), now_ms/1000, now_ms, interval_calc);

        sleep(1);
    }
};

bool testDelayedThread() {
    cout << "in function testDelayedThread" << endl;

    DelayedThreadPool dtp("testDPool", 5);
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
    sleep(12);
    cout << "call stop()" << endl;
    dtp.stop();
    cout << "end of test" << endl;

    return true;
}
