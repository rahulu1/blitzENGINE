//
//  Time.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/14/24.
//

#ifndef Time_hpp
#define Time_hpp

#include <stdint.h>

/**
 *  A high-resolution time-reading and execution-halting class.
 *
 *  A class that provides high-resolution time tracking and execution-halting
 *  functionality using platform-specific calls for maximum precision and accuracy.
 */
class Time {

    friend class Timer;

public:
    
    /**
     *  Uses platform specific calls to pause program execution for a given number of microseconds.
     *
     *  @param  microseconds     the amount of time to pause execution for, in microseconds
     */
    static void Sleep(uint64_t microseconds);
    
    
private:
    /**
     *  Uses platform specific calls to return high-resolution time.
     *
     *  @returns    the current system clock time in microseconds
     */
    static uint64_t GetTime();

};


#endif /* Time_hpp */
