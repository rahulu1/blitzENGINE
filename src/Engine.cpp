#include "Engine.h"

#include "AudioManager.hpp"
#include "TextManager.hpp"
#include "Rigidbody.hpp"

Engine::Engine() : engine_quit(false)
{
    update_timer = Timer();
    
    ComponentManager::Init();
    
    Renderer::Init();
    
    AudioManager::Init();

    ImageManager::Init();
    
    TextManager::Init();
    
    
    ConfigGame();
    
    Renderer::CreateWindow();
    
    SetCurrentScene();
}

void Engine::GameLoop()
{
    while(!engine_quit)
    {
        update_timer.Update();
        
        frame_time = update_timer.GetDeltaTimeSeconds();
        float frame_time_discrepancy = min_frame_time > 0.0f ? min_frame_time - frame_time : -1.0f;
        
        if (frame_time_discrepancy > 0.0f)
            Time::Sleep(frame_time_discrepancy * kMicroPerSecond);
        
        SDL_Event e;
        while(SDL_PollEvent(&e)) { HandleEvent(e); }
        
        FixedUpdate();
        Start();
        Update();
        LateUpdate();
        Input::TransitionInputStates();
        
        RenderFrame();

        if (!name_of_scene_to_load.empty())
            SetCurrentScene();
    }
}


void Engine::FixedUpdate()
{
    if (Rigidbody::GetWorld())
    {
        simulation_time_budget += frame_time;
        
        while (simulation_time_budget > simulation_timestep)
        {
            TweenManager::FixedUpdate();
            Rigidbody::GetWorld()->Step(simulation_timestep, 8, 3);
            current_scene->OnFixedUpdate();
            CollisionManager::ProcessContactCallbacks();
            simulation_time_budget -= simulation_timestep;
            steps_taken_this_frame++;
        }
    }
}


void Engine::ConfigGame()
{
    // Check for required directories
    if (!fs::exists(RESOURCES_PATH))
        ErrorExit("error: resources/ missing");
    else if (!fs::exists(CONFIG_PATH))
        ErrorExit("error: resources/game.config missing");
    
    // Load game config
    rapidjson::Document config_doc;
    ReadJsonFile(CONFIG_PATH, config_doc);
    
    // Check for initial scene in game.config
    if (config_doc.HasMember("initial_scene") && config_doc["initial_scene"].IsString())
        name_of_scene_to_load = config_doc["initial_scene"].GetString();
    else
        ErrorExit("error: initial_scene unspecified");
    
    if (config_doc.HasMember("game_title") && config_doc["game_title"].IsString())
        Renderer::SetGameTitle(config_doc["game_title"].GetString());
    
    if (config_doc.HasMember("physics_timesteps_per_second") && config_doc["physics_timesteps_per_second"].IsNumber())
        simulation_timestep = 1.0f / config_doc["physics_timesteps_per_second"].GetFloat();
    
    if (fs::exists(RENDERING_CONFIG_PATH))
    {
        // Load game config
        rapidjson::Document rendering_config_doc;
        ReadJsonFile(RENDERING_CONFIG_PATH, rendering_config_doc);
        
        if (rendering_config_doc.HasMember("x_resolution"))
            Renderer::SetCameraDimensionsX(rendering_config_doc["x_resolution"].GetInt());
        
        if (rendering_config_doc.HasMember("y_resolution"))
            Renderer::SetCameraDimensionsY(rendering_config_doc["y_resolution"].GetInt());
    
        if (rendering_config_doc.HasMember("clear_color_r"))
            Renderer::SetClearColorR(rendering_config_doc["clear_color_r"].GetInt());
        
        if (rendering_config_doc.HasMember("clear_color_g"))
            Renderer::SetClearColorG(rendering_config_doc["clear_color_g"].GetInt());
        
        if (rendering_config_doc.HasMember("clear_color_b"))
            Renderer::SetClearColorB(rendering_config_doc["clear_color_b"].GetInt());
        
        if (rendering_config_doc.HasMember("max_framerate") && rendering_config_doc["max_framerate"].IsNumber())
            min_frame_time = 1.0f / rendering_config_doc["max_framerate"].GetFloat();
    }
}

void Engine::SetCurrentScene()
{
    SceneManager::SetCurrentScene(name_of_scene_to_load);
    name_of_scene_to_load = "";
    current_scene = &SceneManager::current_scene;
    
    camera_position = glm::vec2(0,0);
}

void Engine::HandleEvent(const SDL_Event &e)
{
    switch (e.type)
    {
        case SDL_QUIT:
            engine_quit = true;
            break;
            
        default:
            Input::ProcessInput(e);
            break;
    }
}
