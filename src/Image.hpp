//
//  Image.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/10/24.
//

#ifndef Image_hpp
#define Image_hpp

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

struct Image
{
    SDL_Texture* texture;
    uint16_t width;
    uint16_t height;
    
    
    Image(SDL_Texture* img_texture, uint16_t img_width, uint16_t img_height) :
    texture(img_texture),
    width(img_width),
    height(img_height) {}
};

#endif /* Image_hpp */
