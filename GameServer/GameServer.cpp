#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <mutex>
#include <thread>
#include <future>
#include "ThreadManager.h"
#include "windows.h"

CoreGlobal Core;

Mutex coutMutex;


void ThreadMain()
{
    while (true)
    {
        std::lock_guard<std::mutex> guard(coutMutex);
        cout << "Hello ! I am thread... " << LThreadId << endl;
        this_thread::sleep_for(chrono::microseconds(50000));
    }
}

int main()
{ 
    for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch(ThreadMain);
	}

	GThreadManager->Join();
	return 0;
}