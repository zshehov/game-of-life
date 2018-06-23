#pragma once
#include <mutex>
#include <stdint.h>
#include <condition_variable>

class ThreadBarrier {

public:
	ThreadBarrier(const uint32_t &threadCount) :
		_threadCount(threadCount),
		_incomingThreads(0),
		_threadsToGoOut(0) {}

	void wait() {
		std::unique_lock<std::mutex> lock(_mutex);

		++_incomingThreads;
		if (_incomingThreads == _threadCount) {
			_incomingThreads = 0;
			_threadsToGoOut = _threadCount - 1; // -1 since the current thread goes out immediately
			_conditionalVar.notify_all();
		} else {
			_conditionalVar.wait(lock, [this]() {return this->_threadsToGoOut != 0; });
			--_threadsToGoOut;
		}

		lock.unlock();
	}

private:
	std::mutex _mutex;
	std::condition_variable _conditionalVar;

	const uint32_t _threadCount;

	uint32_t _incomingThreads;
	uint32_t _threadsToGoOut;
};
