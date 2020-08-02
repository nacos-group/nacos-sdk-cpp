#ifndef __THREAD_H_
#define __THREAD_H_
#include <exception>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include "NacosString.h"
#include "Debug.h"
#define gettidv1() ::syscall(__NR_gettid)  

typedef void *(*ThreadFn)(void *);

class Thread{
private:
	NacosString _threadName;
	pthread_t _thread;
	ThreadFn _function;
	pid_t _tid;
	bool _start;
	void *_threadData;

	Thread() {};
public:
	void setThreadName(const NacosString &threadName) { _threadName = threadName; };
	NacosString getThreadName() { return _threadName; };
	static void *threadFunc(void *param)
	{
		Thread *currentThread = (Thread*)param;
		//currentThread->_tid = gettidv1();

		try
		{
			return currentThread->_function(currentThread->_threadData);
		}
		catch (std::exception &e)
		{
			currentThread->_function = NULL;
			log_error("Exception happens when executing:\n");
			log_error("Thread Name:%s Thread Id:%d\n", currentThread->_threadName.c_str(), currentThread->_tid);
			log_error("Raison:%s", e.what());
			abort();
		}
		catch (...)
		{
			currentThread->_function = NULL;
			log_error("Unknown exception happens when executing:\n");
			log_error("Thread Name:%s Thread Id:%d\n", currentThread->_threadName.c_str(), currentThread->_tid);
			throw;
		}
	}

	Thread(const NacosString &threadName, ThreadFn fn)
	: _threadName(threadName), _function(fn), _threadData(NULL)
	{
		_start = false;
	};

	Thread(const NacosString &threadName, ThreadFn fn, void *threadData)
	: _threadName(threadName), _function(fn), _threadData(threadData)
	{
		_start = false;
	};

	~Thread()
	{
		_start = false;
	}

	void start()
	{
		_start = true;
		pthread_create(&_thread, NULL, threadFunc, (void*)this);
	}

	void join()
	{
		log_debug("Calling Thread::join() on %s\n", _threadName.c_str());
		if (!_start)
		{
			log_debug("Thread::join() called on stopped thread for %s\n", _threadName.c_str());
			return;
		}

		pthread_join(_thread, NULL);
	}
};


#endif
