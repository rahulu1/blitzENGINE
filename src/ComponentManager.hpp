#ifndef ComponentManager_hpp
#define ComponentManager_hpp

#define COMPONENTS_PATH "resources/component_types/"

#include "Component.hpp"

#include "box2d.h"
#include "Tween.hpp"
#include "Utilities.hpp"


#include <memory>
#include <string>
#include <thread>
#include <unordered_map>


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
   //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
    static const std::string open_url_cmd = "start";
   #elif _WIN32
    static const std::string open_url_cmd = "start";
   #endif
#elif __APPLE__
    #include <TargetConditionals.h>
    static const std::string open_url_cmd = "open";
    #if TARGET_IPHONE_SIMULATOR
         // iOS, tvOS, or watchOS Simulator
    #elif TARGET_OS_MACCATALYST
         // Mac's Catalyst (ports iOS API into Mac, like UIKit).
    #elif TARGET_OS_IPHONE
        // iOS, tvOS, or watchOS device
    #elif TARGET_OS_MAC
        // Other kinds of Apple platforms
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __ANDROID__
    // Below __linux__ check should be enough to handle Android,
    // but something may be unique to Android.
#elif __linux__
    static const std::string open_url_cmd = "xdg-open";
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif


class ComponentManager {
    
public:
    
    
    static void Init();
    
    
    static std::shared_ptr<Component> MakeComponent(const std::shared_ptr<Component> &other_component);
    
    
    static std::shared_ptr<Component> MakeComponent(const std::string &component_type);
    
    
    static std::shared_ptr<Component> MakeComponent(const rapidjson::Value &component_json);
    
    
    static void EstablishInheritance(sol::table &instance_table, const std::string &component_type);
    
    
    static void EstablishInheritance(sol::table &instance_table, sol::table &parent_table);
    
    
    static sol::state* GetLuaState();
    
    
//    static void UpdateTweens();
    
private:
    
    
    static void cppApplicationQuit();
    
    
    static void cppApplicationOpenURL(const std::string &url);
    
    
    static void cppDebugLog(const std::string &message);
    
    
    static void cppDebugLogError(const std::string &message);
    
    
    template <typename T>
    static ITween* cppGOTweenTo(sol::table tween_table, sol::protected_function getter, sol::protected_function setter, T end, float duration);
    
    
    static inline sol::state L;
    
};


inline void ComponentManager::EstablishInheritance(sol::table &instance_table, sol::table &parent_table)
{
    instance_table[sol::metatable_key] = L.create_table();
    
    if (instance_table[sol::metatable_key].valid())
        instance_table[sol::metatable_key]["__index"] = parent_table;
}


inline sol::state* ComponentManager::GetLuaState()                             { return &L; }


inline void ComponentManager::cppApplicationQuit()                             { exit(0); }


inline void ComponentManager::cppApplicationOpenURL(const std::string &url)    { std::system((open_url_cmd + " " + url).c_str()); }


inline void ComponentManager::cppDebugLog(const std::string &message)          { std::cout << message << '\n'; }


inline void ComponentManager::cppDebugLogError(const std::string &message)     { std::cerr << message << '\n'; }


#endif /* ComponentManager_hpp */
