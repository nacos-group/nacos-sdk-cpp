#ifndef __THREAD_H_
#define __THREAD_H_

#include <exception>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include "NacosString.h"
#include "src/log/Logger.h"
#include "src/thread/Tid.h"

#define THREAD_STOP_SIGNAL SIGUSR1

namespace nacos{
typedef void *(*ThreadFn)(void *);

/*
* Thread.h
* Author: Liu, Hanyu
* This is NOT like the thread class in Java!
* It's just a simple encapsulation of pthread_create() and pthread_join
* It doesn't have a virtual run() function,
 * a function pointer(ThreadFn) should be passed to the constructor so it will be used as the function pointer parameter for pthread_create
*/
class Thread {
private:
    NacosString _threadName;
    pthread_t _thread;
    ThreadFn _function;
    //TODO:thread id
    TID_T _tid;
    bool _start;
    void *_threadData;

    Thread() {};

    static void empty_signal_handler(int signum) {};
    static struct sigaction old_action;
public:
    static void Init();
    static void DeInit();

    void setThreadName(const NacosString &threadName) { _threadName = threadName; };

    NacosString getThreadName() { return _threadName; };

    static void *threadFunc(void *param);

    Thread(const NacosString &threadName, ThreadFn fn)
            : _threadName(threadName), _function(fn), _threadData(NULL) {
        _start = false;
    };

    Thread(const NacosString &threadName, ThreadFn fn, void *threadData)
            : _threadName(threadName), _function(fn), _threadData(threadData) {
        _start = false;
    };

    ~Thread() {
        _start = false;
    }

    void start();

    void join();

    void kill();
};
}//namespace nacos

#endif
