/*#include "src/thread/DelayedThreadPool.h"

namespace nacos {


DelayedThreadPool::DelayedThreadPool(const NacosString &poolName, long poolSize, long pollingInterval)
:ThreadPool(poolName, poolSize) {
    _pollingInterval = pollingInterval;
}

void *DelayedThreadPool::timerFunc(void *param) {
    DelayedThreadPool *thisObj = (DelayedThreadPool*)param;
    while (!thisObj->_stop) {
        LockGuard __lockSchedTasks(thisObj->_lockForScheduleTasks);

        std::list< std::pair<long, Task*> >::iterator it = thisObj->_scheduledTasks.begin();
        for (
            it != thisObj->_scheduledTasks.end(); it++) {

            }
    }
}

struct ascOrdFunctor {
    bool operator ()(const std::pair<long, Task*> &lhs, const std::pair<long, Task*> &rhs) {
        return lhs.first > rhs.first;
    };
};

void DelayedThreadPool::schedule(Task *t, long futureTimeToRun) {
    std::pair scheduledTask = std::make_pair (0, t);
    {
        LockGuard __lockSchedTasks(_lockForScheduleTasks);
        _scheduledTasks.push_back(scheduledTask);
        std::sort(_scheduledTasks.begin(), _scheduledTasks.end(), ascOrdFunctor::operator());
    }
}

}*/