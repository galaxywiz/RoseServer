#include "stdafx.h"
#include "Thread.h"
#include "Lock.h"

Thread::Thread(thread_t *thread, wstr_t name)
{
	name_ = name;
	thread_ = thread;
	id_ = thread_->get_id();

	ThreadManager::getInstance().put(this);
}

Thread::~Thread()
{
	thread_->join();
	SAFE_DELETE(thread_);
	SAFE_DELETE(lock_);
}

threadId_t Thread::id()
{
	return id_;
}

wstr_t &Thread::name()
{
	return name_;
}

void Thread::setLock(Lock *lock)
{
	lock_ = lock;
}

Lock* Thread::lock()
{
	return lock_;
}

//--------------------------------------------------//
ThreadManager::~ThreadManager()
{
	for (auto thread : threadPool_){
		SAFE_DELETE(thread.second);
	}
}

void ThreadManager::put(Thread *thread)
{
	std::pair<threadId_t, Thread *> node(thread->id(), thread);
	threadPool_.insert(node);
	SLog(L"* create thread : id[0x%X] name[%s], pool size[%d]", thread->id(), thread->name().c_str(), threadPool_.size());
}

void ThreadManager::remove(threadId_t id)
{
	auto iter = threadPool_.find(id);
	if (iter == threadPool_.end()) {
		return;
	}
	auto thread = iter->second;
	threadPool_.erase(iter);
}

Thread* ThreadManager::at(threadId_t id)
{
	if (threadPool_.empty()) {
		return nullptr;
	}
	auto iter = threadPool_.find(id);
	if (iter == threadPool_.end()) {
		return nullptr;
	}
	auto thread = iter->second;
	return thread;
}
