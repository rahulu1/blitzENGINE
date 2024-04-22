//
//  Timer_macos.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#include "Timer.hpp"

Timer::Timer()
{
    previous_update_time = Time::GetTime();
}

void Timer::Update()
{
    uint64_t latest_update_time = Time::GetTime();

    delta_time = latest_update_time - previous_update_time;

    previous_update_time = latest_update_time; 
}

const uint64_t Timer::GetDeltaTimeMicroseconds() { return delta_time / kNanoPerMicro; }

const double Timer::GetDeltaTimeSeconds() { return static_cast<double>(delta_time) / kNanoPerSecond; }