//
//  ImageManager.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/10/24.
//

#include "ImageManager.hpp"

#include "Renderer.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

std::shared_ptr<Image> ImageManager::GetImage(const std::string &image_name)
{
    std::string image_path = IMAGES_PATH + image_name + ".png";
    
    
    if (image_cache.count(image_name) <= 0) // Check if image is already in cache
    {
        if (!fs::exists(image_path)) // if not in cache, check filepath
            ErrorExit("error: missing image " + image_name); // if file does not exist, error
        
        else // if file exists, load texture
        {
            SDL_Texture* get_texture = IMG_LoadTexture(Renderer::GetSDLRenderer(), image_path.c_str());
            AddImage(image_name, get_texture);
        }
    }
    
    return image_cache[image_name];
}


void ImageManager::cppImageDrawEx(const std::string &image_name, float _x, float _y, double _rotation_degrees, float _scale_x, float _scale_y, float _pivot_x, float _pivot_y, float _r, float _g, float _b, float _a, float _sorting_order)
{
    ImageDrawRequest image_draw_req;
    
    image_draw_req.scale_x = _scale_x;
    image_draw_req.scale_y = _scale_y;
    
    image_draw_req.pivot_x = _pivot_x;
    image_draw_req.pivot_y = _pivot_y;
    
    image_draw_req.image = GetImage(image_name);
    
    image_draw_req.x = _x;
    image_draw_req.y = _y;
    
    image_draw_req.rotation_degrees = _rotation_degrees;
    
    image_draw_req.sorting_order = static_cast<uint16_t>(_sorting_order);
    
    image_draw_req.r = static_cast<Uint8>(_r);
    image_draw_req.g = static_cast<Uint8>(_g);
    image_draw_req.b = static_cast<Uint8>(_b);
    image_draw_req.a = static_cast<Uint8>(_a);
    
    Renderer::screenspace_render_requests.push_back(image_draw_req);
}


void ImageManager::cppImageDrawUIEx(const std::string &image_name, float _x, float _y, float _r, float _g, float _b, float _a, float _sorting_order)
{
    ImageDrawRequest image_draw_req;
    
    image_draw_req.image = GetImage(image_name);
    
    image_draw_req.x = _x;
    image_draw_req.y = _y;
    
    image_draw_req.sorting_order = static_cast<uint16_t>(_sorting_order);
    
    image_draw_req.r = static_cast<Uint8>(_r);
    image_draw_req.g = static_cast<Uint8>(_g);
    image_draw_req.b = static_cast<Uint8>(_b);
    image_draw_req.a = static_cast<Uint8>(_a);
    
    image_draw_req.screen_space_mod = 0.0f;
    
    Renderer::ui_render_requests.push_back(image_draw_req);
}
