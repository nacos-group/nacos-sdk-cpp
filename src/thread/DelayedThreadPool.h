#ifndef __DELAYED_THREAD_POOL_H_
#define __DELAYED_THREAD_POOL_H_

#include<list>
#include<utility>
#include "src/thread/ThreadPool.h"
#include "src/thread/Thread.h"
#include "Mutex.h"

namespace nacos {

class DelayedThreadPool : public ThreadPool {
private:
    Mutex _lockForScheduleTasks;//for _scheduledTasks
    std::list< std::pair<long, Task*> > _scheduledTasks;
    Thread *_timerThread;
    static void *timerFunc(void *param);
    DelayedThreadPool();
    long _pollingInterval;
public:    
    DelayedThreadPool(const NacosString &poolName, long poolSize, long pollingInterval);

    /**
    * schedule the execution for a task
    * @param t the task to run
    * @param futureTimeToRun the time (in ms) for the task to run
    */
    void schedule(Task *t, long futureTimeToRun);
};

}

#endif