//
//  EventBus.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/31/24.
//

#include "EventBus.hpp"


void EventBus::LateUpdate()
{
    for (const std::function<void()> &sub_unsub_to_process : sub_unsubs_to_process) { sub_unsub_to_process(); }
    
    sub_unsubs_to_process.clear();
}
