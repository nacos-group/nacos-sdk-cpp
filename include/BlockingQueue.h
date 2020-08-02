#ifndef __BLOCKING_Q_H_
#define __BLOCKING_Q_H_
#include <deque>
#include "Mutex.h"
/*
* BlockingQueue.h
* Thanks to Shuo, Chen's muduo: https://github.com/chenshuo/muduo/blob/master/muduo/base/BlockingQueue.h
*/
/*template<typename T>
class BlockingQueue
{
private:
	Mutex _mutex;
	Condition _notEmpty;
	Condition _notFull;
	std::deque<T> _queue;
	size_t _maxSize;
public:
	BlockingQueue() : _mutex(), _notEmpty(_mutex), _maxSize(64) {};
	BlockingQueue(size_t queueSize) : _mutex(), _notEmpty(_mutex), _maxSize(queueSize) {};
	void enqueue(const T &data)
	{
		LockGuard lockguard(_mutex);
		while (_queue.size() == _maxSize)
		{
			_notFull.wait();
		}
		_queue.push_back(data);
		_notEmpty.notify();
	}

	T dequeue()
	{
		LockGuard lockguard(_mutex);
		while (!(_queue.size() > 0))
		{
			_notEmpty.wait();
		}
		T front = _queue.front();
		_queue.pop_front();
		_notFull.notify();
		return front;
	}
};*/
#endif