#ifndef ImageManager_hpp
#define ImageManager_hpp

#define IMAGES_PATH "resources/images/"

#include "Image.hpp"
#include "Utilities.hpp"

#include <memory>
#include <string>
#include <unordered_map>

class ImageManager {
    
public:
    
    
    static void Init();
    
    
    static std::shared_ptr<Image> GetImage(const std::string &image_name);
    
    
    static std::shared_ptr<Image> AddImage(const std::string &image_name, SDL_Texture *texture);
    
    
    static bool CheckImage(const std::string &image_name);
    
    
    static void cppTest(const std::string &image_name, float _x, float _y, float _rotation_degrees, float _scale_x, float _scale_y, float _pivot_x, float _pivot_y, float _r, float _g, float _b, float _a, float _sorting_order)
    {
        std::cout << image_name << _x << _y << _rotation_degrees << _scale_x << _scale_y << _pivot_x << _pivot_y << _r << _g << _b << _a << _sorting_order << std::endl;
    }
    
    static void cppImageDraw(const std::string &image_name, float _x, float _y);
    
    
    static void cppImageDrawEx(const std::string &image_name, float _x, float _y, double _rotation_degrees, float _scale_x, float _scale_y, float _pivot_x, float _pivot_y, float _r, float _g, float _b, float _a, float _sorting_order);
    
    
    static void cppImageDrawUI(const std::string &image_name, float _x, float _y);
    
    
    static void cppImageDrawUIEx(const std::string &image_name, float _x, float _y, float _r, float _g, float _b, float _a, float _sorting_order);

private:
    
    
    static inline std::unordered_map<std::string, std::shared_ptr<Image>> image_cache;
};


inline void ImageManager::Init()
{
    // Initialize SDL_image for PNG images
    int imgFlags = IMG_INIT_PNG;
    
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        ErrorExit("SDL_image could not initialize! SDL_image Error: " + std::string(IMG_GetError()));
    }
}


inline std::shared_ptr<Image> ImageManager::AddImage(const std::string &image_name, SDL_Texture *texture)
{
    if (image_cache.count(image_name) > 0)
        return image_cache[image_name];
    
    image_cache[image_name] = std::make_shared<Image>(texture, 0, 0);
    
    int get_width;
    int get_height;
    
    SDL_QueryTexture(image_cache[image_name]->texture, NULL, NULL, &get_width, &get_height);
    
    image_cache[image_name]->width = static_cast<uint16_t>(get_width);
    image_cache[image_name]->height = static_cast<uint16_t>(get_height);
    
    return image_cache[image_name];
}


inline bool ImageManager::CheckImage(const std::string &image_name) { return (bool) image_cache.count(image_name); }


inline void ImageManager::cppImageDraw(const std::string &image_name, float _x, float _y)
{
    cppImageDrawEx(image_name, _x, _y, 0, 1, 1, 0.5f, 0.5f, 255, 255, 255, 255, 0);
}


inline void ImageManager::cppImageDrawUI(const std::string &image_name, float _x, float _y) { cppImageDrawUIEx(image_name, _x, _y, 255, 255, 255, 255, 0); }

#endif /* ImageManager_hpp */
