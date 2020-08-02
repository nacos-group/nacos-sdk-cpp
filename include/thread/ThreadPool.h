#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_
#include <list>
#include <deque>
#include "thread/Thread.h"
#include "NacosString.h"
#include "Mutex.h"

class Task
{
private:
	NacosString _taskName;
public:
	virtual void run() = 0;
	virtual ~Task() {};
	void setTaskName(const NacosString &taskName) { _taskName = taskName; };
	NacosString getTaskName() { return _taskName; };
};

class DummyTask : public Task
{
public:
	DummyTask() { setTaskName("DummyTask"); };
	void run() {};
};

class ThreadPool
{
private:
	size_t _poolSize;
	std::list<Thread*> _threads;
	NacosString _poolName;
	std::deque<Task*> _taskList;
	Mutex _lock;
	Condition _NotEmpty;
	Condition _NotFull;
	bool _stop;
	static DummyTask _dummyTask;

	static void *runInThread(void *param);
	ThreadPool():
	_poolSize(0), _poolName("CannotBeCreated"), _NotEmpty(_lock), _NotFull(_lock), _stop(true) {};
public:
	ThreadPool(const NacosString &poolName, size_t poolSize) :
	_poolSize(poolSize), _poolName(poolName), _NotEmpty(_lock), _NotFull(_lock), _stop(true)
	{
	};

	ThreadPool(size_t poolSize) :
	_poolSize(poolSize), _poolName("NacosCliWorkerThread"), _NotEmpty(_lock), _NotFull(_lock), _stop(true)
	{
	};

	Task *take();
	void put(Task *t);
	void start();
	void stop();
};


#endif
