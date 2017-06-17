#pragma once
#include "stdafx.h"
#include <queue>
#include <stdexcept>   

template <typename T>
class ThreadJobQueue
{
private:
    enum {
        WRITE_QUEUE,
        READ_QUEUE,
        MAX_QUEUE,
    };
    std::queue<T>		queue_[MAX_QUEUE];

    std::queue<T>		*writeQueue_;	//input 용 여러 쓰레드에서 입력을 받음
    std::queue<T>		*readQueue_;	//output 용. 한 쓰레드에서 출력을 함

    Lock		        lock_;

public:
    ThreadJobQueue(WCHAR* name)
        : lock_(name)
    {
        writeQueue_ = &queue_[WRITE_QUEUE];
        readQueue_ = &queue_[READ_QUEUE];
    }

	~ThreadJobQueue()
	{
		readQueue_->empty();
		writeQueue_->empty();
	}

    inline void push(const T &t)
    {
        SAFE_LOCK(lock_);
        writeQueue_->push(t);
    }

	inline bool pop(T &t)
	{
		SAFE_LOCK(lock_);
		size_t size = this->size();
		if (size == 0) {
			return false;
		}
		if (readQueue_->empty()) {
			this->swap();
		}
		t = readQueue_->front();
		readQueue_->pop();
		return true;
	}

    inline void swap()
    {
        SAFE_LOCK(lock_);
        if (writeQueue_ == &queue_[WRITE_QUEUE]) {
            writeQueue_ = &queue_[READ_QUEUE];
            readQueue_ = &queue_[WRITE_QUEUE];
        } else {
            writeQueue_ = &queue_[WRITE_QUEUE];
            readQueue_ = &queue_[READ_QUEUE];
        }
    }

    inline bool isEmpty()   { return readQueue_->empty(); }
    inline size_t size()
    {
        SAFE_LOCK(lock_);
		size_t size = (size_t)(queue_[WRITE_QUEUE].size() + queue_[READ_QUEUE].size()); 
		return size;
    }
};