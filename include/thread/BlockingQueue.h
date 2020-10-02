#ifndef __BLOCKING_Q_H_
#define __BLOCKING_Q_H_

#include <deque>
#include "Mutex.h"
/*
* BlockingQueue.h
* Thanks to Shuo, Chen's muduo: https://github.com/chenshuo/muduo/blob/master/muduo/base/BlockingQueue.h
*/
template<typename T>
class BlockingQueue
{
private:
	Mutex _mutex;
	Condition _notEmpty;
	Condition _notFull;
	std::deque<T> _queue;
	size_t _maxSize;
	volatile bool _full;
	volatile bool _empty;
public:

    bool full() {
        LockGuard lockguard(_mutex);
        return _full;
    };
    bool empty() {
        LockGuard lockguard(_mutex);
        return _empty;
    };

	BlockingQueue() : _mutex(), _notEmpty(_mutex), _notFull(_mutex), _maxSize(64), _full(false), _empty(true) {};
	BlockingQueue(size_t queueSize) : _mutex(), _notEmpty(_mutex), _notFull(_mutex), _maxSize(queueSize), _full(false), _empty(true) {};
	void enqueue(const T &data)
	{
		LockGuard lockguard(_mutex);
        _empty = false;
		while (_queue.size() == _maxSize)
		{
		    _full = true;
			_notFull.wait();
		}
		_queue.push_back(data);
		_notEmpty.notify();
	}

	T dequeue()
	{
		LockGuard lockguard(_mutex);
        _full = false;
		while (_queue.empty())
		{
		    _empty = true;
			_notEmpty.wait();
		}
		T front = _queue.front();
		_queue.pop_front();
		_notFull.notify();
		return front;
	}
};
#endif