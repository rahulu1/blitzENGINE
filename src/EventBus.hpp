//
//  EventBus.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/31/24.
//

#ifndef EventBus_hpp
#define EventBus_hpp

#include "Actor.hpp"

#include <stdio.h>

class EventBus {
public:
    
    
    static void cppEventPublish(const std::string &event_type, sol::optional<sol::table> event_object);
    
    
    static void cppEventSubscribe(const std::string &event_type, sol::table component, sol::protected_function function);
    
    
    static void cppEventUnsubscribe(const std::string &event_type, sol::table component, sol::protected_function function);
    
    
    static void LateUpdate();
    
    
    static inline std::vector<std::function<void()>> sub_unsubs_to_process;
    
private:
    
    
    static void EventBusSubscribe(const std::string &event_type, sol::table component, sol::protected_function function);
    
    
    static void EventBusUnsubscribe(const std::string &event_type, sol::table component, sol::protected_function function);
    
    
    static inline std::map<std::string, std::vector<std::pair<sol::protected_function, sol::table>>> event_bus;
};


inline void EventBus::cppEventPublish(const std::string &event_type, sol::optional<sol::table> event_object)
{
    for (std::pair<sol::protected_function, sol::table> &event_subscriber : event_bus[event_type])
    {
        sol::protected_function_result pub_sub_result = (event_subscriber.first)(event_subscriber.second, event_object);
        
        if (!pub_sub_result.valid())
            ReportError("EventBus", pub_sub_result);
    }
}


inline void EventBus::cppEventSubscribe(const std::string &event_type, sol::table component, sol::protected_function function)
{
    sub_unsubs_to_process.emplace_back( [event_type, component, function] { EventBusSubscribe(event_type, component, function); });
}


inline void EventBus::cppEventUnsubscribe(const std::string &event_type, sol::table component, sol::protected_function function)
{
    sub_unsubs_to_process.emplace_back( [event_type, component, function] { EventBusUnsubscribe(event_type, component, function); });
}





inline void EventBus::EventBusSubscribe(const std::string &event_type, sol::table component, sol::protected_function function)
{
    std::pair<sol::protected_function, sol::table> function_component_pair = std::make_pair(function, component);
    event_bus[event_type].emplace_back(function_component_pair);
}


inline void EventBus::EventBusUnsubscribe(const std::string &event_type, sol::table component, sol::protected_function function)
{
    std::pair<sol::protected_function, sol::table> function_component_pair = std::make_pair(function, component);
    event_bus[event_type].erase(std::remove_if(event_bus[event_type].begin(), event_bus[event_type].end(), [function_component_pair](std::pair<sol::protected_function, sol::table> subscriber) { return function_component_pair == subscriber; }));
}

#endif /* EventBus_hpp */
