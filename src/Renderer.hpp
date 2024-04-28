//
//  Renderer.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/6/24.
//

#ifndef Renderer_hpp
#define Renderer_hpp
#define SDL_MAIN_HANDLED

#define PIXELS_PER_METER_ADDEND 99.0f // This should be the target pixels per meter - 1
#define DEBUG_MODE false

#include "Actor.hpp"
#include "glm.hpp"
#include "Image.hpp"
#include "SDL2/SDL.h"
#include "SDL2_ttf/SDL_ttf.h"

#include <stdio.h>
#include <string>
#include <unordered_map>

struct TextDrawRequest
{
    std::shared_ptr<Image> image;
    
    const int x;
    const int y;
};

struct ImageDrawRequest
{
    float x = 0;
    float y = 0;
    
    float scale_x = 1;
    float scale_y = 1;
    
    float pivot_x = 0.5f;
    float pivot_y = 0.5f;
    
    std::shared_ptr<Image> image;
    
    double rotation_degrees = 0;
    uint16_t sorting_order = 0;
    
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    Uint8 a = 255;
    
    float screen_space_mod = 1.0f;
    
    ImageDrawRequest() {}
};

struct PixelDrawRequest
{
    uint32_t order;
    
    int x = 0;
    int y = 0;
    
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = 255;
    Uint8 a = 255;
};


class Renderer
{
    
public:
    
    
    static void Init();
    
    
    static void CreateWindow();
    
    
    static void ClearFrame();
    
    
    static void PresentFrame();
    
    
    static void DrawFrame();
    
    
    static SDL_Renderer* GetSDLRenderer();
    
    
    static void SetGameTitle(const std::string &new_game_title);
    

    static void SetClearColorR(Uint8 new_clear_color_r);
    
    
    static void SetClearColorG(Uint8 new_clear_color_g);
    
    
    static void SetClearColorB(Uint8 new_clear_color_b);
    
    
    static void SetCameraDimensionsX(int new_resolution_x);
    
    
    static void SetCameraDimensionsY(int new_resolution_y);
    
    
    static const glm::ivec2 GetCameraDimensions();
    
    
    static const int GetCameraDimensionsX();
    

    static const int GetCameraDimensionsY();
    
    
    static bool ActorIsOnscreen(Actor &actor, const glm::vec2 &camera_position);
    
    
    static void cppCameraSetZoom(float new_zoom_factor);
    
    
    static float cppCameraGetZoom();
    
    
    static void cppImageDrawPixel(float _x, float _y, float _r, float _g, float _b, float _a);
    
    
    static inline std::vector<ImageDrawRequest> screenspace_render_requests;
    
    
    static inline std::vector<ImageDrawRequest> ui_render_requests;
    
    
    static inline std::queue<TextDrawRequest> text_render_queue;
    
    
    static inline std::vector<PixelDrawRequest> pixel_render_requests;

private:
    
    
    static void DrawScreenSpace();
    
    
    static void DrawUI();
    
    
    static void DrawText();
    
    
    static void DrawPixels();
    
    
    static void DrawImage(ImageDrawRequest image_draw_request);
    
    
    static SDL_RendererFlip GetRendererFlip(bool horizontalFlip, bool verticalFlip);
    
    
    static inline std::string game_title;
    
    
    static inline SDL_Window* game_window;
    
    
    static inline SDL_Renderer* sdl_renderer;
    
    
    static inline float zoom_factor;
    
    
    static inline glm::ivec2 camera_dimensions;
    
    
    static inline int clear_color_r;
    
    
    static inline int clear_color_g;
    
    
    static inline int clear_color_b;
    
};


inline void Renderer::ClearFrame()                                      { SDL_RenderClear(sdl_renderer); }


inline void Renderer::PresentFrame()                                    { SDL_RenderPresent(sdl_renderer); }


inline void Renderer::SetGameTitle(const std::string &new_game_title)   { game_title = new_game_title; }


inline void Renderer::SetCameraDimensionsX(int new_resolution_x)        { camera_dimensions.x = new_resolution_x; }


inline void Renderer::SetCameraDimensionsY(int new_resolution_y)        { camera_dimensions.y = new_resolution_y; }


inline void Renderer::SetClearColorR(Uint8 new_clear_color_r)             { clear_color_r = new_clear_color_r; }


inline void Renderer::SetClearColorG(Uint8 new_clear_color_g)             { clear_color_g = new_clear_color_g; }


inline void Renderer::SetClearColorB(Uint8 new_clear_color_b)             { clear_color_b = new_clear_color_b; }


inline void Renderer::cppCameraSetZoom(float new_zoom_factor)           { zoom_factor = new_zoom_factor; }


inline float Renderer::cppCameraGetZoom()                               { return zoom_factor; }


inline SDL_Renderer* Renderer::GetSDLRenderer()                         { return sdl_renderer; }


inline const glm::ivec2 Renderer::GetCameraDimensions()                 { return camera_dimensions; }


inline const int Renderer::GetCameraDimensionsX()                       { return camera_dimensions.x; }


inline const int Renderer::GetCameraDimensionsY()                       { return camera_dimensions.y; }


#endif /* Renderer_hpp */
