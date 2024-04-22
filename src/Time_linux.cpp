//
//  Time_linux.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/14/24.
//

#include "Time.hpp"

#include <time.h>
#include <unistd.h>

uint64_t Time::GetTime()
{
    timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return (current_time.tv_sec * 1000000) + (current_time.tv_nsec / 1000);
}

void Time::SleepMilliseconds(uint64_t milliseconds) { usleep(static_cast<useconds_t>(milliseconds * 1000)); }
