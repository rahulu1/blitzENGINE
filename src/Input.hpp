//
//  Input.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/15/24.
//

#ifndef Input_hpp
#define Input_hpp

#include "glm.hpp"
#include "SDL2/SDL.h"

#include <iostream>
#include <queue>
#include <stdio.h>
#include <string>
#include <unordered_map>


enum INPUT_STATE { UNINIT, INPUT_STATE_UP, INPUT_STATE_JUST_BECAME_DOWN, INPUT_STATE_DOWN, INPUT_STATE_JUST_BECAME_UP, INPUT_STATE_JUST_BECAME_DOWN_AND_UP, INPUT_STATE_JUST_BECAME_UP_AND_DOWN };


class Input
{
public:
    static void Init(); // Call before main loop begins.
    static void ProcessInput(const SDL_Event & e); // Call every frame at start of event loop.
    static void TransitionInputStates();

    static bool cppInputGetKey(const std::string &keycode);
    static bool cppInputGetKeyDown(const std::string &keycode);
    static bool cppInputGetKeyUp(const std::string &keycode);
    
    static bool cppInputGetMouseButton(Uint8 mouse_button);
    static bool cppInputGetMouseButtonDown(Uint8 mouse_button);
    static bool cppInputGetMouseButtonUp(Uint8 mouse_button);
    static glm::vec2 cppInputGetMousePosition();
    static float cppInputGetMouseScrollDelta();
    
    static bool cppInputGetButton(const std::string &button_name);
    static bool cppInputGetButtonDown(const std::string &button_name);
    static bool cppInputGetButtonUp(const std::string &button_name);
    
    static float cppGetAxis(const std::string &axis_name);
    
    static glm::vec2 cppInputGetControllerTouchpadPosition();

private:
    static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyboard_states;
    static inline std::queue<SDL_Scancode> just_changed_scancodes;
    
    static inline std::unordered_map<Uint8, INPUT_STATE> mouse_button_states;
    static inline std::queue<Uint8> just_changed_mouse_buttons;
    static inline glm::vec2 mouse_position;
    static inline float mouse_scroll_delta;
    
    static inline std::unordered_map<SDL_GameControllerButton, INPUT_STATE> controller_button_states;
    static inline std::queue<Uint8> just_changed_controller_buttons;
    
    static inline std::unordered_map<SDL_GameControllerAxis, float> controller_axis_states;
    static inline glm::vec2 controller_touchpad_position;
};

#endif /* Input_hpp */
