#ifndef __MUTEX_H_
#define __MUTEX_H_

#include<iostream>
#include <pthread.h>
#include "Tid.h"
#include "src/utils/TimeUtils.h"

/*
* Mutex.h
* Author: Liu, Hanyu
* Thanks to Shuo, Chen's muduo:
* https://github.com/chenshuo/muduo/blob/master/muduo/base/Mutex.h
*/

namespace nacos{
class Mutex {
    friend class Condition;

private:
    TID_T _holder;
    pthread_mutex_t _mutex;
public:
    Mutex() { pthread_mutex_init(&_mutex, NULL); };

    ~Mutex() { pthread_mutex_destroy(&_mutex); };

    void lock() {
        pthread_mutex_lock(&_mutex);
        assignHolder();
    };

    void unlock() {
        unassignHolder();
        pthread_mutex_unlock(&_mutex);
    };

    pthread_mutex_t *getPthreadMutex() { return &_mutex; };

    void assignHolder() { _holder = gettidv1(); };

    void unassignHolder() { _holder = 0; };
};

class Condition {
private:
    Mutex &_mutex;
    pthread_cond_t _cond;
public:
    Condition(Mutex &mutex) : _mutex(mutex) { pthread_cond_init(&_cond, NULL); };

    ~Condition() { pthread_cond_destroy(&_cond); };

    int wait() {
        return pthread_cond_wait(&_cond, _mutex.getPthreadMutex());
    }

    int wait(long millis) {
        struct timeval now;
        struct timespec wakeup_time;

        TimeUtils::getCurrentTimeInStruct(now);
        now.tv_usec = now.tv_usec + millis * 1000;
        now.tv_sec = now.tv_sec + now.tv_usec / 1000000;
        now.tv_usec = now.tv_usec % 1000000;

        wakeup_time.tv_nsec = now.tv_usec * 1000;
        wakeup_time.tv_sec = now.tv_sec;
        //std::cout << " millis:" << millis
        //<< "   wakeup time:sec:" << wakeup_time.tv_sec << "  nsec:" << wakeup_time.tv_nsec << std::endl;

        return pthread_cond_timedwait(&_cond, _mutex.getPthreadMutex(), &wakeup_time);
    }

    void notify() {
        pthread_cond_signal(&_cond);
    }

    void notifyAll() {
        pthread_cond_broadcast(&_cond);
    }
};

class LockGuard {
private:
    Mutex &_mutex;
public:
    LockGuard(Mutex &mutex) : _mutex(mutex) { _mutex.lock(); };

    ~LockGuard() { _mutex.unlock(); };
};
}//namespace nacos

#endif
