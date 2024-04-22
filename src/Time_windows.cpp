//
//  Time_windows.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/14/24.
//

#include "Time.hpp"
#include <windows.h>

uint64_t Time::GetTime()
{
    LARGE_INTEGER frequency, currentTime;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&currentTime);
    return (currentTime.QuadPart * 1000000) / frequency.QuadPart;
}

void Time::SleepMilliseconds(uint64_t milliseconds) { Sleep(static_cast<DWORD>(milliseconds)); }
