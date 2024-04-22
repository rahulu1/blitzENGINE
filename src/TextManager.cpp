//
//  TextManager.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/10/24.
//

#include "TextManager.hpp"

#include "ImageManager.hpp"
#include "Renderer.hpp"

#include <iostream>
#include <filesystem>


namespace fs = std::filesystem;


TTF_Font* TextManager::GetFont(const std::string &font_name, int font_size)
{
    std::string font_key = font_name + std::to_string(font_size);
    
    auto it = font_cache.find(font_key);
    if (it == font_cache.end()) {
        std::string font_path = FONTS_PATH + font_name + ".ttf";
        if(!fs::exists(font_path))
            ErrorExit("error: font " + font_name + " missing");
        
        TTF_Font* font = TTF_OpenFont(font_path.c_str(), font_size);
        if (!font) {
            std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
            exit(1);
        }
        font_cache[font_key] = font;
        return font;
    }
    return it->second;
}

void TextManager::cppTextDraw(const std::string &str_content, float _x, float _y, const std::string &font_name, float _font_size, float _r, float _g, float _b, float _a)
{
    int font_size = static_cast<int>(_font_size);
    TTF_Font* font_ptr = GetFont(font_name, font_size);
    
    Uint8 r = static_cast<Uint8>(_r);
    Uint8 g = static_cast<Uint8>(_g);
    Uint8 b = static_cast<Uint8>(_b);
    Uint8 a = static_cast<Uint8>(_a);
    
    SDL_Color font_color{r, g, b, a};
    
    std::string texture_key = CreateTextureKey(font_ptr, str_content, r, g, b, a);
    
    Image* text_image;
    
    if (ImageManager::CheckImage(texture_key))
        text_image = ImageManager::GetImage(texture_key);
    else // If texture for this text doesn't already exist, create new texture
    {
        SDL_Surface* surface = TTF_RenderText_Solid(font_ptr, str_content.c_str(), font_color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::GetSDLRenderer(), surface);
        SDL_FreeSurface(surface);
        
        text_image = ImageManager::AddImage(texture_key, texture);
    }
    
    int x = static_cast<int>(_x);
    int y = static_cast<int>(_y);
    
    TextDrawRequest text_draw_req = {text_image, x, y};
    
    Renderer::text_render_queue.push(text_draw_req);
}
