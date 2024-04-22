//
//  TextManager.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/10/24.
//

#ifndef TEXTMANAGER_HPP
#define TEXTMANAGER_HPP

#define FONTS_PATH "resources/fonts/"
#define DEFAULT_FONT_SIZE 16
#define DEFAULT_FONT_COLOR {255, 255, 255, 255}

#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "Utilities.hpp"

#include <unordered_map>
#include <map>
#include <string>


class TextManager {
public:
    
    
    static void Init();
    
    
    static TTF_Font* GetFont(const std::string &font_name, int font_size);
    
    
    static void cppTextDraw(const std::string &str_content, float _x, float _y, const std::string &font_name, float _font_size, float _r, float _g, float _b, float _a);

private:
    
    
    static std::string CreateTextureKey(TTF_Font* font_ptr, const std::string &str_content, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    
    
    static inline std::unordered_map<std::string, TTF_Font*> font_cache;
    
};


inline void TextManager::Init()
{
    
    if (TTF_Init() == -1)
        ErrorExit("TTF could not initialize: " + std::string(TTF_GetError()));
}


inline std::string TextManager::CreateTextureKey(TTF_Font* font_ptr, const std::string &str_content, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    std::stringstream ss;
    ss << font_ptr << "_" << str_content << "_" << r << "_" << g << "_" << b << "_" << a;
    return ss.str();
}

#endif // TEXTMANAGER_HPP
