//
//  SceneManager.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 1/31/24.
//

#ifndef SceneManager_hpp
#define SceneManager_hpp

#include <unordered_map>

#include "Scene.hpp"
#include "Template.hpp"

class SceneManager {
public:
    
    /**
     * Creates and initializes a new Scene object from a .scene file with the provided scene name.
     *
     * @params scene_to_set The name of the .scene file to load the Scene from.
     */
    static void SetCurrentScene(const std::string &scene_to_set);
    
    static Actor* cppActorFind(const std::string &actor_name);
    
    static sol::table cppActorFindAll(const std::string &actor_name);
    
    static Actor* cppActorInstantiate(const std::string &actor_template_name);
    
    static void cppActorDestroy(Actor* actor_to_destroy);
    
    static void cppSceneDontDestroy(Actor* actor);
    
    static std::string cppSceneGetCurrent();
    
    static inline std::unordered_map<std::string, Template> actor_templates;
    
    static inline std::map<int, std::shared_ptr<Actor>> dont_destroy_on_load;
    
    static inline Scene current_scene;
};


inline Actor* SceneManager::cppActorFind(const std::string &actor_name)     { return current_scene.FindActor(actor_name); }


inline sol::table SceneManager::cppActorFindAll(const std::string &actor_name)  { return current_scene.FindAllActors(actor_name); }


inline void SceneManager::cppActorDestroy(Actor* actor_to_destroy) { current_scene.DestroyActor(actor_to_destroy); }


inline std::string SceneManager::cppSceneGetCurrent() { return current_scene.scene_name; }

#endif /* SceneManager_hpp */
