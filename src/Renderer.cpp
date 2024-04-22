//
//  Renderer.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/6/24.
//

#include "Renderer.hpp"

#include "Engine.h"
#include <cmath>

const bool ImageDrawRequestComp(const ImageDrawRequest &lhs, const ImageDrawRequest &rhs) { return lhs.sorting_order < rhs.sorting_order; }

const bool PixelDrawRequestComp(const PixelDrawRequest &lhs, const PixelDrawRequest &rhs)
{
    if (lhs.r != rhs.r) return lhs.r < rhs.r;
    if (lhs.g != rhs.g) return lhs.g < rhs.g;
    if (lhs.b != rhs.b) return lhs.b < rhs.b;
    if (lhs.a != rhs.a) return lhs.a < rhs.a;
    return lhs.order < rhs.order; // Ensure drawing order for same location
}

static inline int pixels_requested = 0;

void Renderer::Init()
{
    SDL_Init(SDL_INIT_VIDEO);
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        ErrorExit("SDL_Init Error: " + std::string(SDL_GetError()));
    
    game_window = nullptr;
    sdl_renderer = nullptr;
    game_title = "";
    zoom_factor = 1;
    camera_dimensions.x = 640;
    camera_dimensions.y = 360;
    clear_color_r = 255;
    clear_color_g = 255;
    clear_color_b = 255;
}

void Renderer::CreateWindow()
{
    game_window = SDL_CreateWindow(game_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, camera_dimensions.x, camera_dimensions.y, SDL_WINDOW_SHOWN);
    sdl_renderer = SDL_CreateRenderer(game_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(sdl_renderer, clear_color_r, clear_color_g, clear_color_b, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(sdl_renderer);
}

void Renderer::DrawFrame()
{
    DrawScreenSpace();
    DrawUI();
    DrawText();
    DrawPixels();
}

void Renderer::DrawScreenSpace()
{
    if (screenspace_render_requests.empty())
        return;
    
    SDL_RenderSetScale(sdl_renderer, zoom_factor, zoom_factor);
    
    std::stable_sort(screenspace_render_requests.begin(), screenspace_render_requests.end(), ImageDrawRequestComp);
    
    for (ImageDrawRequest &screenspace_render_request : screenspace_render_requests)
    {
        DrawImage(screenspace_render_request);
    }
    
    for (ImageDrawRequest &screenspace_render_request : screenspace_render_requests)
    {
        SDL_SetTextureColorMod(screenspace_render_request.image->texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(screenspace_render_request.image->texture, 255);
    }
    
    screenspace_render_requests.clear();
    
    SDL_RenderSetScale(sdl_renderer, 1, 1);
}

void Renderer::DrawUI()
{
    if (ui_render_requests.empty())
        return;
    
    std::stable_sort(ui_render_requests.begin(), ui_render_requests.end(), ImageDrawRequestComp);
    
    for (ImageDrawRequest &ui_render_request : ui_render_requests)
    {
        DrawImage(ui_render_request);
    }
    
    for (ImageDrawRequest &ui_render_request : screenspace_render_requests)
    {
        SDL_SetTextureColorMod(ui_render_request.image->texture, 255, 255, 255);
        SDL_SetTextureAlphaMod(ui_render_request.image->texture, 255);
    }
    
    ui_render_requests.clear();
}


void Renderer::DrawText()
{
    while (!text_render_queue.empty())
    {
        TextDrawRequest current_request = text_render_queue.front();
        
        SDL_Rect renderQuad{current_request.x, current_request.y, current_request.image->width, current_request.image->height};
        SDL_RenderCopy(sdl_renderer, current_request.image->texture, NULL, &renderQuad);
        
        text_render_queue.pop();
    }
}

void Renderer::DrawPixels()
{
    if (pixel_render_requests.empty())
        return;
    
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);
    
    std::stable_sort(pixel_render_requests.begin(), pixel_render_requests.end(), PixelDrawRequestComp);
    
    SDL_Color current_color = {pixel_render_requests.front().r, pixel_render_requests.front().g, pixel_render_requests.front().b, pixel_render_requests.front().a};
    
    std::vector<SDL_Point> batch_points; // Temporary storage for batch drawing

    for (const PixelDrawRequest &current_request : pixel_render_requests)
    {
        // If color changes draw the batch and start a new one
        if ((current_request.r != current_color.r || current_request.g != current_color.g ||
             current_request.b != current_color.b || current_request.a != current_color.a))
        {
            SDL_SetRenderDrawColor(sdl_renderer, current_color.r, current_color.g, current_color.b, current_color.a);
            SDL_RenderDrawPoints(sdl_renderer, batch_points.data(), static_cast<int>(batch_points.size()));
            batch_points.clear();
            
            // Update current color
            current_color = {current_request.r, current_request.g, current_request.b, current_request.a};
        }
        
        // Add pixel to the batch
        batch_points.push_back(SDL_Point{current_request.x, current_request.y});
    }

    // Draw any remaining pixels in the final batch
    if (!batch_points.empty())
    {
        SDL_SetRenderDrawColor(sdl_renderer, current_color.r, current_color.g, current_color.b, current_color.a);
        SDL_RenderDrawPoints(sdl_renderer, batch_points.data(), static_cast<int>(batch_points.size()));
    }
    
    pixel_render_requests.clear();
    
    SDL_SetRenderDrawColor(sdl_renderer, clear_color_r, clear_color_g, clear_color_b, SDL_ALPHA_OPAQUE);
    SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_NONE);
}

void Renderer::DrawImage(ImageDrawRequest current_request)
{
    float pixels_per_meter = 1.0f + (current_request.screen_space_mod * PIXELS_PER_METER_ADDEND);
    
    glm::vec2 final_rendering_position = glm::vec2(current_request.x, current_request.y) - (Engine::GetCameraPosition() * current_request.screen_space_mod);
    
    SDL_Rect dstrect;
    dstrect.w = current_request.image->width;
    dstrect.h = current_request.image->height;
    
    bool flip_x = current_request.scale_x < 0.0f;
    bool flip_y = current_request.scale_y < 0.0f;
    SDL_RendererFlip flip = GetRendererFlip(flip_x, flip_y);
    
    dstrect.w *= std::abs(current_request.scale_x);
    dstrect.h *= std::abs(current_request.scale_y);
    
    SDL_Point center = {static_cast<int>(current_request.pivot_x * dstrect.w), static_cast<int>(current_request.pivot_y * dstrect.h)};
    
    dstrect.x = static_cast<int>(final_rendering_position.x *  pixels_per_meter + camera_dimensions.x * 0.5f * (1.0f / zoom_factor) * current_request.screen_space_mod - center.x * current_request.screen_space_mod);
    dstrect.y = static_cast<int>(final_rendering_position.y *  pixels_per_meter + camera_dimensions.y * 0.5f * (1.0f / zoom_factor) * current_request.screen_space_mod - center.y * current_request.screen_space_mod);
    
    SDL_Rect screen_rect;
    
    screen_rect.x = 0;
    screen_rect.y = 0;
    screen_rect.w = Renderer::GetCameraDimensionsX();
    screen_rect.h = Renderer::GetCameraDimensionsY();
    
    SDL_Color current_mod;
    SDL_GetTextureColorMod(current_request.image->texture, &current_mod.r, &current_mod.g, &current_mod.b);
    SDL_GetTextureAlphaMod(current_request.image->texture, &current_mod.a);
    
    if (current_mod.r != current_request.r || current_mod.g != current_request.g || current_mod.b != current_request.b)
        SDL_SetTextureColorMod(current_request.image->texture, current_request.r, current_request.g, current_request.b);
    
    if (current_mod.a != current_request.a)
        SDL_SetTextureAlphaMod(current_request.image->texture, current_request.a);
    
    SDL_RenderCopyEx(sdl_renderer, current_request.image->texture, NULL, &dstrect, static_cast<double>(current_request.rotation_degrees), &center, flip);
}

void Renderer::cppImageDrawPixel(float _x, float _y, float _r, float _g, float _b, float _a)
{
    int x = static_cast<int>(_x);
    int y = static_cast<int>(_y);

    Uint8 r = static_cast<Uint8>(_r);
    Uint8 g = static_cast<Uint8>(_g);
    Uint8 b = static_cast<Uint8>(_b);
    Uint8 a = static_cast<Uint8>(_a);
    
    PixelDrawRequest pixel_draw_req = {pixels_requested++, x, y, r, g, b, a};
    
    pixel_render_requests.push_back(pixel_draw_req);
}

SDL_RendererFlip Renderer::GetRendererFlip(bool horizontalFlip, bool verticalFlip)
{
    int flip = SDL_FLIP_NONE;
    
    if (horizontalFlip)
        flip |= SDL_FLIP_HORIZONTAL;
    if (verticalFlip)
        flip |= SDL_FLIP_VERTICAL;
    
    return static_cast<SDL_RendererFlip>(flip);
}
