#include <algorithm>
#include "src/thread/DelayedThreadPool.h"
#include "NacosExceptions.h"

namespace nacos {

/**
* A thread pool that can run tasks with specified delay
*/
class DelayedWorker : public Task {
private:
    DelayedThreadPool &_container;
public:
    DelayedWorker(DelayedThreadPool &container) : _container(container) {

    };

    void run() {
        log_debug("DelayedWorker::run()\n");
        while (!_container._stop_delayed_tp) {
            _container._lockForScheduleTasks.lock();

            //no data, wait until data is available
            log_debug("[DelayedWorker] start to wait for task stop=%d\n", _container._stop);
            if (_container._scheduledTasks.empty()) {
                log_debug("[DelayedWorker] empty, wait for task\n");
                _container._delayTaskNotEmpty.wait();
                _container._lockForScheduleTasks.unlock();
                if (_container._stop_delayed_tp) {
                    return;
                }
                log_debug("[DelayedWorker] awake continue\n");
                continue;
            } else {//
                log_debug("[DelayedWorker] iterating on _scheduledTasks\n");
                std::vector< std::pair<long, Task*> >::iterator it = _container._scheduledTasks.begin();
                while (it != _container._scheduledTasks.end()) {
                    int64_t now_time = TimeUtils::getCurrentTimeInMs();
                    log_debug("[DelayedWorker] now = %ld wakeup time = %ld\n", now_time, it->first);
                    if (it->first <= now_time) {
                        Task *task = it->second;
                        _container._scheduledTasks.erase(it);
                        _container._lockForScheduleTasks.unlock();
                        //the task can also attempt to retrieve the lock
                        task->run();
                        _container._lockForScheduleTasks.lock();
                        log_debug("[DelayedWorker] continue 2 next task\n");
                        continue;
                    } else {
                        _container._delayTaskNotEmpty.wait(it->first - now_time);
                        //awake from sleep when a stop signal is sent
                        if (_container._stop_delayed_tp) {
                            _container._lockForScheduleTasks.unlock();
                            return;
                        }
                        it = _container._scheduledTasks.begin();
                        _container._lockForScheduleTasks.unlock();
                        continue;
                    }
                }
            }
        }
    }
};

DelayedThreadPool::DelayedThreadPool(const NacosString &poolName, size_t poolSize)
:ThreadPool(poolName, poolSize),_delayTaskNotEmpty(_lockForScheduleTasks), _stop_delayed_tp(true) {
    log_debug("DelayedThreadPool::DelayedThreadPool() name = %s size = %d\n", poolName.c_str(), poolSize);
    if (poolSize <= 0) {
        throw NacosException(NacosException::INVALID_PARAM, "Poll size cannot be lesser than 0");
    }
    delayTasks = new Task*[poolSize];
    log_debug("DelayedThreadPool::DelayedThreadPool initializing tasks\n");
    for (size_t i = 0; i < poolSize; i++) {
        delayTasks[i] = new DelayedWorker(*this);
    }
}


DelayedThreadPool::~DelayedThreadPool() {
    log_debug("DelayedThreadPool::~DelayedThreadPool\n");
    if (delayTasks != NULL) {
        for (size_t i = 0; i < _poolSize; i++) {
            delete delayTasks[i];
            delayTasks[i] = NULL;
        }
        delete delayTasks;
        delayTasks = NULL;
    }
}

struct tagAscOrdFunctor{
    bool operator ()(const std::pair<long, Task*> &lhs, const std::pair<long, Task*> &rhs) {
        return lhs.first < rhs.first;
    };
} ascOrdFunctor;

//futureTimeToRun: time in MS
void DelayedThreadPool::schedule(Task *t, long futureTimeToRun) {
    if (_stop) {
        return;
    }
    log_debug("DelayedThreadPool::schedule() name=%s future = %d\n", t->getTaskName().c_str(), futureTimeToRun);
    std::pair<long, Task*> scheduledTask = std::make_pair (futureTimeToRun, t);
    {
        LockGuard __lockSchedTasks(_lockForScheduleTasks);
        _scheduledTasks.push_back(scheduledTask);
        std::sort(_scheduledTasks.begin(), _scheduledTasks.end(), ascOrdFunctor);
        _delayTaskNotEmpty.notifyAll();
    }
}

void DelayedThreadPool::start() {
    _stop_delayed_tp = false;
    ThreadPool::start();
    log_debug("DelayedThreadPool::start()\n");
    for (size_t i = 0; i < _poolSize; i++) {
        put(delayTasks[i]);
    }
}

void DelayedThreadPool::stop() {
    if (_stop_delayed_tp) {
        return;
    }

    _stop_delayed_tp = true;

    for (std::list<Thread *>::iterator it = _threads.begin(); it != _threads.end(); it++) {
        (*it)->kill();
    }

    ThreadPool::stop();
}

}