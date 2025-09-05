//#include <pch.h>
//#include <atomic>
//#include <mutex>
//#include <windows.h>
//#include <future>
//
////[][][][][][][][] [][][][][][][]  [][][][][][]
//// 가져올 떄 값 하나만 가져오는게 아니라 블록 단위로 가져온다.
//// 그리고 캐시에 저장 -> 나중에 옆에 값을 가져온다면 캐시 히트
//
//using namespace std;
//
//int32 buffer[10000][10000];
//
//int main()
//{
//    memset(buffer, 0, sizeof(buffer));
//
//    {
//        uint64 start = GetTickCount64();
//        int64 sum = 0;
//        for (int32 i = 0; i < 10000; i++)
//            for (int32 j = 0l; j < 10000; j++)
//                sum += buffer[i][j];
//
//        uint64 end = GetTickCount64();
//        cout << "Elapsed Tick" << (end - start) << endl;
//    }
//
//
//    {
//        uint64 start = GetTickCount64();
//        int64 sum = 0;
//        for (int32 i = 0; i < 10000; i++)
//            for (int32 j = 0l; j < 10000; j++)
//                sum += buffer[j][i];
//
//        uint64 end = GetTickCount64();
//        cout << "Elapsed Tick" << (end - start) << endl;
//    }
//    // 인접한 단위 블록을 가져오는데 하지만 옆에 데이터를 쓰는게 아니라
//    // 다른 데이터를 사용하고 있다.
//    // 캐시 데이터를 피해 사용하고 있다. -> 그렇다고 캐시 메모리에 히트 안하는 건 아니지만
//    // 명중률이 내려간다. 
//}


#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <mutex>
#include <thread>
#include "LockFreeStack.h"
#include "LockFreeStack2.h"

LockFreeStack<int32> s;
LockFreeStack2<int32> s2;


void Push()

{
	while (true) {
		int32 value = rand() % 100;
		s2.Push(value);
		this_thread::sleep_for(chrono::milliseconds(100));
		
	}
}

void Pop()
{
	/*while (true) {
		int32 data = 0;
		if (s.TryPop(data)) {
			std::cout << data << std::endl;
		}
	}*/

	while (true) {
		auto data = s2.TryPop();
		if(data!= nullptr)
			std::cout << *data << std::endl;
	}
}

int main()
{
	/*shared_ptr<int32> ptr;
	bool value = atomic_is_lock_free(&ptr);*/

	std::thread t1(Push);
	std::thread t2(Pop);
	std::thread t3(Pop);
	t1.join();
	t2.join();
	t3.join();
}