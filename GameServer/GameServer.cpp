#include <pch.h>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>

//[][][][][][][][] [][][][][][][]  [][][][][][]
// 가져올 떄 값 하나만 가져오는게 아니라 블록 단위로 가져온다.
// 그리고 캐시에 저장 -> 나중에 옆에 값을 가져온다면 캐시 히트

using namespace std;

int32 buffer[10000][10000];

int main()
{
    memset(buffer, 0, sizeof(buffer));

    {
        uint64 start = GetTickCount64();
        int64 sum = 0;
        for (int32 i = 0; i < 10000; i++)
            for (int32 j = 0l; j < 10000; j++)
                sum += buffer[i][j];

        uint64 end = GetTickCount64();
        cout << "Elapsed Tick" << (end - start) << endl;
    }


    {
        uint64 start = GetTickCount64();
        int64 sum = 0;
        for (int32 i = 0; i < 10000; i++)
            for (int32 j = 0l; j < 10000; j++)
                sum += buffer[j][i];

        uint64 end = GetTickCount64();
        cout << "Elapsed Tick" << (end - start) << endl;
    }
    // 인접한 단위 블록을 가져오는데 하지만 옆에 데이터를 쓰는게 아니라
    // 다른 데이터를 사용하고 있다.
    // 캐시 데이터를 피해 사용하고 있다. -> 그렇다고 캐시 메모리에 히트 안하는 건 아니지만
    // 명중률이 내려간다. 
}