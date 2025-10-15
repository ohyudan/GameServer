#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <mutex>
#include <thread>
#include <future>
#include "windows.h"
#include "ThreadManager.h"

#include "PlayerManager.h"
#include "AccountManager.h"

//
//Mutex coutMutex;
//
//
//void ThreadMain()
//{
//    while (true)
//    {
//        std::lock_guard<std::mutex> guard(coutMutex);
//        cout << "Hello ! I am thread... " << LThreadId << endl;
//        this_thread::sleep_for(chrono::microseconds(50000));
//    }
//}
//
//int main()
//{ 
//    for (int32 i = 0; i < 5; i++)
//	{
//		GThreadManager->Launch(ThreadMain);
//	}
//
//	GThreadManager->Join();
//	return 0;
//}


//class TestLock {
//
//	USE_LOCK;
//
//public :
//	int32 TestRead() {
//		READ_LOCK;
//
//		if(_queue.empty())
//			return -1;
//
//		return _queue.front();
//	}
//
//	void TestPush() {	
//		WRITE_LOCK;
//
//		_queue.push(rand() % 100);
//
//	}
//
//	void TestPop() {
//		WRITE_LOCK;
//
//		if (_queue.empty() == false)
//			_queue.pop();	
//	}
//private:
//	queue<int32> _queue;
//};
// 
//TestLock testLock;
/*void ThreadWrite() {
	while (true) 
	{
		testLock.TestPush();
		this_thread::sleep_for(chrono::microseconds(600000));
		testLock.TestPop();		
	}
}

void ThreadRead() {
	while (true) 
	{
		int32 value = testLock.TestRead();
		cout << "Read Value : " << value << endl;
		this_thread::sleep_for(chrono::microseconds(700000));
	}
}*/
int main() {

	GThreadManager->Launch([=] {
		while (true) {	
			cout << "PlayerThenAccount " << endl;
			GPlayerManager.PlayerThenAccount();
			this_thread::sleep_for(chrono::microseconds(1000*100));
		}
	});

	GThreadManager->Launch([=] {
		while (true) {
			cout << "AccountThenPlayer " << endl;
			GAccountManager.AccountThenPlayer();
			this_thread::sleep_for(chrono::microseconds(1000*100));
		}
		});
	
	GThreadManager->Join();

}