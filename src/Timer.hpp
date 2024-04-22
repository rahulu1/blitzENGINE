//
//  Timer.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#ifndef Timer_hpp
#define Timer_hpp

#include "Time.hpp"

constexpr uint64_t kNanoPerMicro = 1e3;

constexpr double kNanoPerSecond = 1e9;

constexpr uint64_t kMicroPerSecond = 1e6;

class Timer {
public:
    
    Timer();

    /**
     *  Updates timestamp and delta_time since last Timer::Update.
     */
    void Update();
    
    /**
     *  Returns the delta_time in microseconds (seconds * 10-e6).
     *
     *  @return the time between latest Timer::Update and the one before that in microseconds
     */
    const uint64_t GetDeltaTimeMicroseconds();
    
    
    /**
     *  Returns the time elapsed since the last Update in seconds.
     *
     *  @return the time between latest Timer::Update and the one before that in seconds
     */
    const double GetDeltaTimeSeconds();

private:
        
    uint64_t previous_update_time = 0;
    
    uint64_t delta_time = 0;
};

#endif /* Timer_hpp */
