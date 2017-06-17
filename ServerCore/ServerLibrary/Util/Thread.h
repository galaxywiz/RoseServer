#pragma once
#include "stdafx.h"

#define MAKE_THREAD(className, process)	(new Thread(new thread_t(&className##::##process, this), L#className))
#define GET_CURRENT_THREAD_ID		std::this_thread::get_id
class Lock;
typedef std::function<void(void *)> ThreadFunction;

class Thread
{
	threadId_t				id_;
	wstr_t					name_;
	thread_t				*thread_;
	Lock					*lock_;			//지금 걸린 락
	
public:
	Thread(thread_t *thread, wstr_t name);
	~Thread();	

	threadId_t id();
	wstr_t &name();

	void setLock(Lock *lock);
	Lock *lock();
};

//#define THREAD_POOL_HASHMAP
class ThreadManager : public Singleton < ThreadManager >
{
	// HACK: hash_map / unordered_map 에서 get를 할때, 라이브러리에서 버켓 index 에러가 난다.
	// HACK: 그런 이유로 검증된 map으로 컨테이너 교체를 한다.
#ifdef THREAD_POOL_HASHMAP
	hash_map <threadId_t, Thread*> threadPool_;
#else //THREAD_POOL_HASHMAP
	map <threadId_t, Thread *> threadPool_;
#endif //THREAD_POOL_HASHMAP

public:
	~ThreadManager();

	void put(Thread *thread);
	void remove(threadId_t id);
	Thread *at(threadId_t id);
};
