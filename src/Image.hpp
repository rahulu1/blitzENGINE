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
    int width;
    int height;
};

#endif /* Image_hpp */
