#include "pch.h"
#include "ThreadManager.h"
#include "Types.h" 
#include "CoreTLS.h"

ThreadManager::ThreadManager()
{
}

ThreadManager::~ThreadManager()
{
    Join();
}

void ThreadManager::Launch(std::function<void()> callback)
{
    std::lock_guard<Mutex> guard(_lock);
    _threads.push_back(thread([=]() {
		InitTLS();
		callback();
		DestroyTLS();
        }));
}

void ThreadManager::Join()
{
    for (thread& t: _threads) {
        if(t.joinable())
			t.join();
    }
	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThreadId = 0;
    LThreadId=	SThreadId.fetch_add(1);
}

void ThreadManager::DestroyTLS()
{
    // TLS 해제 코드 필요시 구현
}
