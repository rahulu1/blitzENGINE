//
//  Input.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/15/24.
//

#include "Input.hpp"

std::unordered_map<std::string, SDL_Scancode> __keycode_to_scancode = {
    // Directional (arrow) Keys
    {"up", SDL_SCANCODE_UP},
    {"down", SDL_SCANCODE_DOWN},
    {"right", SDL_SCANCODE_RIGHT},
    {"left", SDL_SCANCODE_LEFT},

    // Misc Keys
    {"escape", SDL_SCANCODE_ESCAPE},

    // Modifier Keys
    {"lshift", SDL_SCANCODE_LSHIFT},
    {"rshift", SDL_SCANCODE_RSHIFT},
    {"lctrl", SDL_SCANCODE_LCTRL},
    {"rctrl", SDL_SCANCODE_RCTRL},
    {"lalt", SDL_SCANCODE_LALT},
    {"ralt", SDL_SCANCODE_RALT},

    // Editing Keys
    {"tab", SDL_SCANCODE_TAB},
    {"return", SDL_SCANCODE_RETURN},
    {"enter", SDL_SCANCODE_RETURN},
    {"backspace", SDL_SCANCODE_BACKSPACE},
    {"delete", SDL_SCANCODE_DELETE},
    {"insert", SDL_SCANCODE_INSERT},

    // Character Keys
    {"space", SDL_SCANCODE_SPACE},
    {"a", SDL_SCANCODE_A},
    {"b", SDL_SCANCODE_B},
    {"c", SDL_SCANCODE_C},
    {"d", SDL_SCANCODE_D},
    {"e", SDL_SCANCODE_E},
    {"f", SDL_SCANCODE_F},
    {"g", SDL_SCANCODE_G},
    {"h", SDL_SCANCODE_H},
    {"i", SDL_SCANCODE_I},
    {"j", SDL_SCANCODE_J},
    {"k", SDL_SCANCODE_K},
    {"l", SDL_SCANCODE_L},
    {"m", SDL_SCANCODE_M},
    {"n", SDL_SCANCODE_N},
    {"o", SDL_SCANCODE_O},
    {"p", SDL_SCANCODE_P},
    {"q", SDL_SCANCODE_Q},
    {"r", SDL_SCANCODE_R},
    {"s", SDL_SCANCODE_S},
    {"t", SDL_SCANCODE_T},
    {"u", SDL_SCANCODE_U},
    {"v", SDL_SCANCODE_V},
    {"w", SDL_SCANCODE_W},
    {"x", SDL_SCANCODE_X},
    {"y", SDL_SCANCODE_Y},
    {"z", SDL_SCANCODE_Z},
    {"0", SDL_SCANCODE_0},
    {"1", SDL_SCANCODE_1},
    {"2", SDL_SCANCODE_2},
    {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5},
    {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7},
    {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9},
    {"/", SDL_SCANCODE_SLASH},
    {";", SDL_SCANCODE_SEMICOLON},
    {"=", SDL_SCANCODE_EQUALS},
    {"-", SDL_SCANCODE_MINUS},
    {".", SDL_SCANCODE_PERIOD},
    {",", SDL_SCANCODE_COMMA},
    {"[", SDL_SCANCODE_LEFTBRACKET},
    {"]", SDL_SCANCODE_RIGHTBRACKET},
    {"\\", SDL_SCANCODE_BACKSLASH},
    {"'", SDL_SCANCODE_APOSTROPHE}
};

std::unordered_map<std::string, SDL_GameControllerButton> __button_name_to_sdl_button = {
    /* Face Buttons (ABXY) */
    {"North", SDL_CONTROLLER_BUTTON_A},
    {"East", SDL_CONTROLLER_BUTTON_B},
    {"South", SDL_CONTROLLER_BUTTON_X},
    {"West", SDL_CONTROLLER_BUTTON_Y},
    
    /*  */
    {"Back", SDL_CONTROLLER_BUTTON_BACK},
    {"Guide", SDL_CONTROLLER_BUTTON_GUIDE},
    {"Start", SDL_CONTROLLER_BUTTON_START},
    
    /* Joysticks Buttons */
    {"LeftStick", SDL_CONTROLLER_BUTTON_LEFTSTICK},
    {"RightStick", SDL_CONTROLLER_BUTTON_RIGHTSTICK},
    
    /* Shoulder Buttons */
    {"LeftShoulder", SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    {"RightShoulder", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
    
    /* D-Pad Buttons */
    {"Up", SDL_CONTROLLER_BUTTON_DPAD_UP},
    {"Down", SDL_CONTROLLER_BUTTON_DPAD_DOWN},
    {"Right", SDL_CONTROLLER_BUTTON_DPAD_LEFT},
    {"Left", SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
    
    /* Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button */
    {"Misc1", SDL_CONTROLLER_BUTTON_MISC1},
    
    /* Platform-specific Buttons */
    {"Paddle1", SDL_CONTROLLER_BUTTON_PADDLE1},  /* Xbox Elite paddle P1 (upper left, facing the back) */
    {"Paddle2", SDL_CONTROLLER_BUTTON_PADDLE2},  /* Xbox Elite paddle P3 (upper right, facing the back) */
    {"Paddle3", SDL_CONTROLLER_BUTTON_PADDLE3},  /* Xbox Elite paddle P2 (lower left, facing the back) */
    {"Paddle4", SDL_CONTROLLER_BUTTON_PADDLE4},  /* Xbox Elite paddle P4 (lower right, facing the back) */
    {"Touchpad", SDL_CONTROLLER_BUTTON_TOUCHPAD} /* PS4/PS5 touchpad button */
};

std::unordered_map<std::string, SDL_GameControllerAxis> __axis_name_to_sdl_axis = {
    {"LeftX", SDL_CONTROLLER_AXIS_LEFTX},
    {"LeftY", SDL_CONTROLLER_AXIS_LEFTY},
    {"RightX", SDL_CONTROLLER_AXIS_RIGHTX},
    {"RightY", SDL_CONTROLLER_AXIS_RIGHTY},
    {"LeftTrigger", SDL_CONTROLLER_AXIS_TRIGGERLEFT},
    {"RightTrigger", SDL_CONTROLLER_AXIS_TRIGGERRIGHT}
};



void Input::Init()
{
    SDL_Init(SDL_INIT_EVENTS);
    
    keyboard_states.clear();
    
    mouse_button_states.clear();
    mouse_position = glm::vec2(0,0);
    mouse_scroll_delta = 0.0f;
    
    controller_button_states.clear();
    controller_axis_states.clear();
    controller_touchpad_position = glm::vec2(0,0);
}

void Input::ProcessInput(const SDL_Event &e)
{
    switch (e.type)
    {
        case SDL_KEYDOWN: {
            SDL_Scancode scancode = e.key.keysym.scancode;
            
            // If first time being pressed, or currently up, set to down
            if (keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_UP || keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
            {
                keyboard_states[scancode] = INPUT_STATE_JUST_BECAME_UP_AND_DOWN;
            }
            else
            {
                keyboard_states[scancode] = INPUT_STATE_JUST_BECAME_DOWN;
                just_changed_scancodes.push(scancode);
            }
                
            break;
        }
            
        case SDL_KEYUP: {
            SDL_Scancode scancode = e.key.keysym.scancode;
            
            
            if (keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_DOWN || keyboard_states[scancode] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN)
            {
                keyboard_states[scancode] = INPUT_STATE_JUST_BECAME_DOWN_AND_UP;
            }
            else
            {
                keyboard_states[scancode] = INPUT_STATE_JUST_BECAME_UP;
                just_changed_scancodes.push(scancode);
            }
                
            break;
        }
            
        case SDL_MOUSEBUTTONDOWN: {
            Uint8 mouse_button = e.button.button;
            
            // If first time being pressed, or currently up, set to down
            if (mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_UP || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
            {
                mouse_button_states[mouse_button] = INPUT_STATE_JUST_BECAME_UP_AND_DOWN;
            }
            else
            {
                mouse_button_states[mouse_button] = INPUT_STATE_JUST_BECAME_DOWN;
                just_changed_mouse_buttons.push(mouse_button);
            }
            break;
        }
            
        case SDL_MOUSEBUTTONUP: {
            Uint8 mouse_button = e.button.button;
            
            // If first time being pressed, or currently up, set to down
            if (mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_DOWN || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN)
            {
                mouse_button_states[mouse_button] = INPUT_STATE_JUST_BECAME_DOWN_AND_UP;
            }
            else
            {
                mouse_button_states[mouse_button] = INPUT_STATE_JUST_BECAME_UP;
                just_changed_mouse_buttons.push(mouse_button);
            }
            break;
        }
            
        case SDL_MOUSEMOTION: {
            mouse_position.x = e.motion.x;
            mouse_position.y = e.motion.y;
            
            break;
        }
            
        case SDL_MOUSEWHEEL: {
            mouse_scroll_delta = e.wheel.preciseY;
            
            break;
        }
            
        case SDL_CONTROLLERBUTTONDOWN: {
            SDL_GameControllerButton controller_button = (SDL_GameControllerButton) e.cbutton.button;
            
            // If first time being pressed, or currently up, set to down
            if (controller_button_states[controller_button] == INPUT_STATE_JUST_BECAME_UP || controller_button_states[controller_button] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
            {
                controller_button_states[controller_button] = INPUT_STATE_JUST_BECAME_UP_AND_DOWN;
            }
            else
            {
                mouse_button_states[controller_button] = INPUT_STATE_JUST_BECAME_DOWN;
                just_changed_controller_buttons.push(controller_button);
            }
            break;
        }
            
        case SDL_CONTROLLERBUTTONUP: {
            SDL_GameControllerButton controller_button = (SDL_GameControllerButton) e.cbutton.button;
            
            // If first time being pressed, or currently up, set to down
            if (controller_button_states[controller_button] == INPUT_STATE_JUST_BECAME_DOWN || controller_button_states[controller_button] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN)
            {
                controller_button_states[controller_button] = INPUT_STATE_JUST_BECAME_DOWN_AND_UP;
            }
            else
            {
                controller_button_states[controller_button] = INPUT_STATE_JUST_BECAME_UP;
                just_changed_controller_buttons.push(controller_button);
            }
            break;
        }
            
        case SDL_CONTROLLERTOUCHPADDOWN: {

            break;
        }
            
        case SDL_CONTROLLERTOUCHPADUP: {
            
            break;
        }
            
        case SDL_CONTROLLERTOUCHPADMOTION: {
            controller_touchpad_position.x = e.motion.x;
            controller_touchpad_position.y = e.motion.y;
            
            break;
        }
            
        // If not a mouse or keyboard input, do nothing
        default:
            break;
    }
}

bool Input::cppInputGetKey(const std::string &keycode)
{
    if(__keycode_to_scancode.count(keycode) <= 0)
        return false;
    
    // If key currently is up, return false
    if (keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_DOWN || keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_DOWN || keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN)
        return true;
    
    return false;
}

bool Input::cppInputGetKeyDown(const std::string &keycode)
{
    if(__keycode_to_scancode.count(keycode) <= 0)
        return false;
    
    if (keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_DOWN || keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN || keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
        return true;
    
    return false;
}

bool Input::cppInputGetKeyUp(const std::string &keycode)
{
    if(__keycode_to_scancode.count(keycode) <= 0)
        return false;
    
    if (keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_UP || keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN || keyboard_states[__keycode_to_scancode[keycode]] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
        return true;
    
    return false;
}

bool Input::cppInputGetMouseButton(Uint8 mouse_button)
{
    // If key currently is up, return false
    if (mouse_button_states[mouse_button] == INPUT_STATE_DOWN || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_DOWN || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN)
        return true;
    
    return false;
}

bool Input::cppInputGetMouseButtonDown(Uint8 mouse_button)
{
    if (mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_DOWN || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
        return true;
    
    return false;
}

bool Input::cppInputGetMouseButtonUp(Uint8 mouse_button)
{
    if (mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_UP || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN || mouse_button_states[mouse_button] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
        return true;
    
    return false;
}

glm::vec2 Input::cppInputGetMousePosition() { return mouse_position; }

float Input::cppInputGetMouseScrollDelta()  { return mouse_scroll_delta; }

void Input::TransitionInputStates()
{
    while(!just_changed_scancodes.empty())
    {
        switch (keyboard_states[just_changed_scancodes.front()]) {
            case INPUT_STATE_JUST_BECAME_UP: case INPUT_STATE_JUST_BECAME_DOWN_AND_UP:
                keyboard_states[just_changed_scancodes.front()] = INPUT_STATE_UP;
                break;
                
            case INPUT_STATE_JUST_BECAME_DOWN: case INPUT_STATE_JUST_BECAME_UP_AND_DOWN:
                keyboard_states[just_changed_scancodes.front()] = INPUT_STATE_DOWN;
                break;
                
            default:
                break;
        }
        
        just_changed_scancodes.pop();
    }
    while(!just_changed_mouse_buttons.empty())
    {
        switch (mouse_button_states[just_changed_mouse_buttons.front()]) {
            case INPUT_STATE_JUST_BECAME_UP: case INPUT_STATE_JUST_BECAME_DOWN_AND_UP:
                mouse_button_states[just_changed_mouse_buttons.front()] = INPUT_STATE_UP;
                break;
                
            case INPUT_STATE_JUST_BECAME_DOWN: case INPUT_STATE_JUST_BECAME_UP_AND_DOWN:
                mouse_button_states[just_changed_mouse_buttons.front()] = INPUT_STATE_DOWN;
                break;
                
            default:
                break;
        }
        
        just_changed_mouse_buttons.pop();
    }
    
    mouse_scroll_delta = 0;
}

bool Input::cppInputGetButton(const std::string &button_name)
{
    if(__button_name_to_sdl_button.count(button_name) <= 0)
        return false;
    
    // If key currently is up, return false
    if (controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_DOWN || controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_DOWN || controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN)
        return true;
    
    return false;
}

bool Input::cppInputGetButtonDown(const std::string &button_name)
{
    if(__button_name_to_sdl_button.count(button_name) <= 0)
        return false;
    
    if (controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_DOWN || controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN || controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
        return true;
    
    return false;
}

bool Input::cppInputGetButtonUp(const std::string &button_name)
{
    if(__button_name_to_sdl_button.count(button_name) <= 0)
        return false;
    
    if (controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_UP || controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_UP_AND_DOWN || controller_button_states[__button_name_to_sdl_button[button_name]] == INPUT_STATE_JUST_BECAME_DOWN_AND_UP)
        return true;
    
    return false;
}
