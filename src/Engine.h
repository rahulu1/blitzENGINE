#ifndef ENGINE_H
#define ENGINE_H

#define DEBUG_FPS false

#define RESOURCES_PATH "resources"
#define CONFIG_PATH  "resources/game.config"
#define RENDERING_CONFIG_PATH "resources/rendering.config"

#include <cstdlib>
#include <filesystem>
#include <string>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include "Actor.hpp"
#include "EventBus.hpp"
#include "ImageManager.hpp"
#include "Input.hpp"
#include "Renderer.hpp"
#include "SceneManager.hpp"
#include "TweenManager.hpp"
#include "Timer.hpp"


namespace fs = std::filesystem;


class Engine
{
public:
    
    
    Engine();

    
    void ConfigGame();
    
    
	void GameLoop();
    
    
    static glm::vec2 GetCameraPosition();
    
    
    static double GetDeltaTime();


    static float GetFixedDeltaTime();
    
    
    static void cppCameraSetPosition(float x, float y);
    
    
    static float cppCameraGetPositionX();
    
    
    static float cppCameraGetPositionY();
    
    
    static void cppSceneLoad(const std::string &scene_name);
    
    
    static inline Timer update_timer;

private:
    
    
    void Start();
    
    
    void Update();
    
    
    void FixedUpdate();
    
    
    void LateUpdate();
    
    
    void RenderFrame();
    
    
    void SetCurrentScene();
    
    
    void HandleEvent(const SDL_Event &e);
    
    
    static inline Scene *current_scene;
    
    
    static inline std::string name_of_scene_to_load;
    
    
    static inline glm::vec2 camera_position = glm::vec2(0.0f, 0.0f);
    
    /**
     *  The minimum time between frame renders during runtime.
     *
     *  Will be automatically calculated from max framerate if set in
     *  the engine's config file.
     *
     *  If set to a negative value, renders at an uncapped framerate.
     */
    static inline float min_frame_time = -1.0f;
    
    
    static inline float frame_time = 0.0f;
    
    
    static inline float simulation_timestep = 1.0f / 120.0f;
    
    
    static inline float simulation_time_budget = 0.0f;
    
    
    static inline int steps_taken_this_frame = 0;
    
    
    bool engine_quit = false;
    
};


inline void Engine::Start()     { current_scene->OnStart(); }


inline void Engine::Update()
{
    TweenManager::Update();
    current_scene->OnUpdate();
}


inline void Engine::LateUpdate()
{
    TweenManager::LateUpdate();
    current_scene->OnLateUpdate();
    EventBus::LateUpdate();
    current_scene->OnDestroy();
}


inline void Engine::RenderFrame()
{
    steps_taken_this_frame = 0;
    Renderer::ClearFrame();
    Renderer::DrawFrame();
    Renderer::PresentFrame();
    
#if DEBUG_FPS
    std::cout << "FPS, Steps Taken: " << (1.0f / frame_time) << ", " << steps_taken_this_frame << '\n';
#endif /*DEBUG_FPS*/
}


inline glm::vec2 Engine::GetCameraPosition()                    { return camera_position; }


inline double Engine::GetDeltaTime()                            { return update_timer.GetDeltaTimeSeconds(); }


inline float Engine::GetFixedDeltaTime()                        { return simulation_timestep; }


inline void Engine::cppCameraSetPosition(float x, float y)      { camera_position = glm::vec2(x, y); }


inline float Engine::cppCameraGetPositionX()                    { return GetCameraPosition().x; }


inline float Engine::cppCameraGetPositionY()                    { return GetCameraPosition().y; }


inline void Engine::cppSceneLoad(const std::string &scene_name) { name_of_scene_to_load = scene_name; }


#endif // !ENGINE_H



