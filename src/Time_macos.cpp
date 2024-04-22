//
//  Time_macos.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/14/24.
//

#include "Time.hpp"

#include <mach/mach_time.h>
#include <unistd.h>


uint64_t Time::GetTime()
{
    static mach_timebase_info_data_t timebase;
    
    if (timebase.denom == 0)
        mach_timebase_info(&timebase);
    
    return mach_absolute_time() * timebase.numer / timebase.denom;
}


void Time::Sleep(uint64_t microseconds) { usleep(static_cast<useconds_t>(microseconds)); }
