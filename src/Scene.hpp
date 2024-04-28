//
//  Scene.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 1/29/24.
//

#ifndef Scene_hpp
#define Scene_hpp

#define SCENES_PATH "resources/scenes/"

#include "Actor.hpp"
#include "document.h"
#include "Utilities.hpp"

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>


namespace fs = std::filesystem;

class Scene
{
    friend class SceneManager;
    
public:
    
    
    void Init(const std::string &new_scene_name);
    
    
    void OnStart();
    
    
    void OnUpdate();
    
    
    void OnLateUpdate();
    
    
    void OnFixedUpdate();
    
    
    void OnDestroy();
    
    
    Actor* FindActor(const std::string &actor_name);
    
    
    sol::table FindAllActors(const std::string &actor_name);
    
    
    std::shared_ptr<Actor> DontDestroy(Actor* actor);
    
    
    std::map<std::string, std::set<uint32_t>> uuids_by_name;

    
    std::map<uint32_t, std::shared_ptr<Actor>> actors_by_uuid;
    
    
    std::set<std::shared_ptr<Actor>, Actor::less> actors_destroying_components;

    
    std::vector<std::shared_ptr<Actor>> starting_actors_to_add;
    
    
    std::string scene_name;
    
private:
    
    std::shared_ptr<Actor> QueueActorToAdd(const std::shared_ptr<Actor> &actor);
    
    
    void DestroyActor(Actor *actor);
    
    
    void BatchEraseDeadActorsFromMap(std::map<uint32_t, std::weak_ptr<Actor>> &flat_map_to_clean);
    
    
    void AddNewActors();
    
    
    void AddActorToSceneLifecycleMaps(const std::shared_ptr<Actor> &actor);
    
    
    std::map<uint32_t, std::weak_ptr<Actor>> starting_actors;
    
    
    std::map<uint32_t, std::weak_ptr<Actor>> updating_actors;
    
    
    std::map<uint32_t, std::weak_ptr<Actor>> late_updating_actors;
    
    
    std::map<uint32_t, std::weak_ptr<Actor>> fixed_updating_actors;
    
    
    std::string scene_path;
    
    
    static inline const float removal_threshold = 0.2f;
};


inline std::shared_ptr<Actor> Scene::DontDestroy(Actor *actor)
{
    if (actors_by_uuid.count(actor->uuid) > 0)
        return actors_by_uuid[actor->uuid];
    
    return std::shared_ptr<Actor>(nullptr);
}


inline std::shared_ptr<Actor> Scene::QueueActorToAdd(const std::shared_ptr<Actor> &actor)
{
    actor->SetSelfReference(actor);
    
    starting_actors_to_add.emplace_back(actor);
    
    return actor;
}


inline void Scene::AddActorToSceneLifecycleMaps(const std::shared_ptr<Actor> &actor)
{
    if (!actor->updating_components.empty())
        updating_actors.try_emplace(actor->uuid, actor);
    
    if (!actor->late_updating_components.empty())
        late_updating_actors.try_emplace(actor->uuid, actor);
    
    if (!actor->fixed_updating_components.empty())
        fixed_updating_actors.try_emplace(actor->uuid, actor);
}

#endif /* Scene_hpp */
