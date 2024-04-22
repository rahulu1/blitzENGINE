//
//  Scene.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 1/29/24.
//

#include "Scene.hpp"

#include "Rigidbody.hpp"
#include "Time.hpp"

void Scene::Init(const std::string &new_scene_name)
{
    scene_name = new_scene_name;
    scene_path = SCENES_PATH + scene_name + ".scene";
    
    if (!fs::exists(scene_path))
       ErrorExit("error: scene " + scene_name + " is missing");
    
    // Read scene JSON
    rapidjson::Document scene_doc;
    ReadJsonFile(scene_path, scene_doc);
    
    // Get array of actors in scene
    const rapidjson::Value& actors_json = scene_doc["actors"];
    assert(actors_json.IsArray());
    
    // Iterate through and load actors in scene
    for (auto &actor_data : actors_json.GetArray())
    {
        std::shared_ptr<Actor> actor;
        
        std::string actor_template_name;
        
        // If actor uses template, copy template values
        if (actor_data.HasMember("template") && actor_data["template"].IsString())
        {
            actor_template_name = actor_data["template"].GetString();
            Actor &template_actor = SceneManager::actor_templates.try_emplace(actor_template_name, actor_template_name).first->second.template_actor;
            actor = std::make_shared<Actor>(template_actor);
        }
        else
            actor = std::make_shared<Actor>();
        
        actor->UpdateActorWithJSON(actor_data);
        
        QueueActorToAdd(actor);
    }
}

void Scene::OnStart()
{
    AddNewActors();
    
    for (auto starting_actor_pair : starting_actors)
    {
        if (std::shared_ptr<Actor> actor_sp = starting_actor_pair.second.lock())
            actor_sp->OnStart();
    }
    
    starting_actors.clear();
}

void Scene::OnUpdate()
{
    float dead_actors = 0;
    for (auto actor_pair : updating_actors)
    {
        std::weak_ptr<Actor> actor = actor_pair.second;
        if (std::shared_ptr<Actor> actor_sp = actor.lock())
            actor_sp->OnUpdate();
        else
            ++dead_actors;
    }
    
    if (dead_actors / static_cast<float>(updating_actors.size()) >= removal_threshold)
        BatchEraseDeadActorsFromMap(updating_actors);
}

void Scene::OnLateUpdate()
{
    float dead_actors = 0;
    for (auto actor_pair : late_updating_actors)
    {
        std::weak_ptr<Actor> actor = actor_pair.second;
        if (std::shared_ptr<Actor> actor_sp = actor.lock())
            actor_sp->OnLateUpdate();
        else
            ++dead_actors;
    }
    
    if (dead_actors / static_cast<float>(late_updating_actors.size()) >= removal_threshold)
        BatchEraseDeadActorsFromMap(late_updating_actors);
}


void Scene::OnFixedUpdate()
{
    float dead_actors = 0;
    for (auto actor_pair : fixed_updating_actors)
    {
        std::weak_ptr<Actor> actor = actor_pair.second;
        if (std::shared_ptr<Actor> actor_sp = actor.lock())
            actor_sp->OnFixedUpdate();
        else
            ++dead_actors;
    }
    
    if (dead_actors / static_cast<float>(fixed_updating_actors.size()) >= removal_threshold)
        BatchEraseDeadActorsFromMap(fixed_updating_actors);
}


void Scene::OnDestroy()
{
    std::set<std::shared_ptr<Actor>, Actor::less> actors_calling_on_destroy(std::move(actors_destroying_components));
    
    for (std::shared_ptr<Actor> actor_calling_on_destroy : actors_calling_on_destroy)
    {
        actor_calling_on_destroy->OnDestroy();
    }
}


Actor* Scene::FindActor(const std::string &actor_name)
{
    if (uuids_by_name.count(actor_name) > 0)
    {
        auto actor_by_name_it = uuids_by_name[actor_name].begin();
        
        return actors_by_uuid[*actor_by_name_it].get();
    }
    
    auto starting_actors_to_add_it = std::find_if(starting_actors_to_add.begin(), starting_actors_to_add.end(),
                                                  [actor_name](const std::shared_ptr<Actor> &starting_actor_to_add) {
        return starting_actor_to_add->name == actor_name; });
    
    if (starting_actors_to_add_it != starting_actors_to_add.end())
        return starting_actors_to_add_it->get();

    return nullptr;
}


sol::table Scene::FindAllActors(const std::string &actor_name)
{
    sol::table actor_table = ComponentManager::GetLuaState()->create_table();
    int actors_found = 0;
    
    if (uuids_by_name.count(actor_name) > 0 && !uuids_by_name[actor_name].empty())
    {
        for (int actor_by_name : uuids_by_name[actor_name])
        {
            actor_table[++actors_found] = actors_by_uuid[actor_by_name].get();
        }
    }
    
    for (std::shared_ptr<Actor> starting_actor_to_add : starting_actors_to_add)
    {
        if (starting_actor_to_add->name == actor_name)
            actor_table[++actors_found] = starting_actor_to_add.get();
    }
    
    return actor_table;
}


void Scene::DestroyActor(Actor *actor)
{
    if (actor)
    {
        actor->RemoveAllComponents();
        
        SceneManager::dont_destroy_on_load.erase(actor->uuid);
        
        if (actors_by_uuid.count(actor->uuid) > 0)
        {
            if (!actor->starting_components.empty())
                starting_actors[actor->uuid].reset();
            
            if (!actor->updating_components.empty())
                updating_actors[actor->uuid].reset();
            
            if (!actor->late_updating_components.empty())
                late_updating_actors[actor->uuid].reset();
            
            if (!actor->fixed_updating_components.empty())
                fixed_updating_actors[actor->uuid].reset();
            
            uuids_by_name[actor->name].erase(actor->uuid);
            
            actors_by_uuid.erase(actor->uuid);
        }
        else
            starting_actors_to_add.erase(std::remove_if(starting_actors_to_add.begin(), starting_actors_to_add.end(), [actor](std::shared_ptr<Actor> &starting_actor_to_add) { return starting_actor_to_add->uuid == actor->uuid; }));
    }
}


void Scene::BatchEraseDeadActorsFromMap(std::map<int, std::weak_ptr<Actor>> &flat_map_to_clean)
{
    for (auto actor_it = flat_map_to_clean.begin(); actor_it != flat_map_to_clean.end();)
    {
        if (std::shared_ptr<Actor> actor = actor_it->second.lock())
            ++actor_it;
        else
            actor_it = flat_map_to_clean.erase(actor_it);
    }
}


void Scene::AddNewActors()
{
    for (std::weak_ptr<Actor> starting_actor_to_add : starting_actors_to_add)
    {
        if (std::shared_ptr<Actor> actor_sp = starting_actor_to_add.lock())
        {
            actors_by_uuid.try_emplace(actor_sp->uuid, actor_sp);
            
            uuids_by_name[actor_sp->name].emplace(actor_sp->uuid);
            
            starting_actors.try_emplace(actor_sp->uuid, starting_actor_to_add);
            
            actor_sp->AddComponents();
            
            AddActorToSceneLifecycleMaps(actor_sp);
        }
    }
    
    starting_actors_to_add.clear();
}
