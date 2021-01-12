#include "Thread.h"

using namespace nacos;

struct sigaction Thread::old_action;

void Thread::Init() {
    struct sigaction action;

    action.sa_flags = 0;
    action.sa_handler = empty_signal_handler;
    sigemptyset(&action.sa_mask);

    sigaction(THREAD_STOP_SIGNAL, &action, &Thread::old_action);
};

void Thread::DeInit() {
    sigaction(THREAD_STOP_SIGNAL, &Thread::old_action, NULL);
};

void *Thread::threadFunc(void *param) {
    Thread *currentThread = (Thread *) param;
    currentThread->_tid = gettidv1();

    try {
        return currentThread->_function(currentThread->_threadData);
    }
    catch (std::exception &e) {
        currentThread->_function = NULL;
        log_error("Exception happens when executing:\n");
        log_error("Thread Name:%s Thread Id:%d\n", currentThread->_threadName.c_str(), currentThread->_tid);
        log_error("Raison:%s", e.what());
        abort();
    }
    catch (...) {
        currentThread->_function = NULL;
        log_error("Unknown exception happens when executing:\n");
        log_error("Thread Name:%s Thread Id:%d\n", currentThread->_threadName.c_str(), currentThread->_tid);
        throw;
    }
}

void Thread::start() {
    _start = true;
    pthread_create(&_thread, NULL, threadFunc, (void *) this);
}

void Thread::join() {
    log_debug("Calling Thread::join() on %s\n", _threadName.c_str());
    if (!_start) {
        log_debug("Thread::join() called on stopped thread for %s\n", _threadName.c_str());
        return;
    }

    pthread_join(_thread, NULL);
}

void Thread::kill() {
    pthread_kill(_thread, THREAD_STOP_SIGNAL);
}