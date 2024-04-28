//
//  Actor.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 1/29/24.
//

#include "Actor.hpp"

#include "AudioManager.hpp"
#include "EventBus.hpp"
#include "ImageManager.hpp"
#include "SceneManager.hpp"

#include <algorithm>


Actor::Actor(const Actor &other)
    :   name(other.name),
        uuid(next_uuid++)
{
    for(auto &other_actor_component_pair : other.actor_components)
    {
        std::shared_ptr<Component> copied_component = ComponentManager::MakeComponent(other_actor_component_pair.second);
        QueueComponentToAdd(copied_component);
    }
    
    AddComponents();
}

void Actor::SetSelfReference(const std::shared_ptr<Actor> &actor_ptr)
{
    self_reference = actor_ptr;
    
    for (auto actor_component_pair : actor_components)
    {
        actor_component_pair.second->SetActor(self_reference);
    }
}


void Actor::UpdateActorWithJSON(const rapidjson::Value &actor_data)
{
    name = actor_data.HasMember("name") && actor_data["name"].IsString() ? actor_data["name"].GetString() : name;
    
    if (actor_data.HasMember("components"))
    {
        for (const auto &component_json : actor_data["components"].GetObject())
        {
            std::string component_key = component_json.name.GetString();
            if (actor_components.count(component_key) <= 0)
            {
                std::shared_ptr new_component = ComponentManager::MakeComponent(component_json.value);
                QueueComponentToAdd(new_component)->SetComponentKey(component_key);
            }
            else
                actor_components[component_key]->UpdateComponentWithJSON(component_json.value);
        }
    }
    
    AddComponents();
}


void Actor::OnStart()
{
    AddComponents();
    
    for (auto &actor_component_pair : starting_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnStart();
        else
        {
            updating_components.erase(actor_component_pair.first);
            late_updating_components.erase(actor_component_pair.first);
        }
    }
    
    starting_components.clear();
}


void Actor::OnUpdate()
{
    float dead_components = 0.0f;
    for (auto &actor_component_pair : updating_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnUpdate();
        else
            ++dead_components;
    }
    
    if (dead_components / static_cast<float>(updating_components.size()) >= removal_threshold)
        BatchEraseDeadComponentsFromMap(updating_components);
}


void Actor::OnLateUpdate()
{
    float dead_components = 0.0f;
    for (auto &actor_component_pair : late_updating_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnLateUpdate();
        else
            ++dead_components;
    }
    
    if (dead_components / static_cast<float>(late_updating_components.size()) >= removal_threshold)
        BatchEraseDeadComponentsFromMap(late_updating_components);
}


void Actor::OnFixedUpdate()
{
    float dead_components = 0.0f;
    for (auto &actor_component_pair : fixed_updating_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnFixedUpdate();
        else
            ++dead_components;
    }
    
    if (dead_components / static_cast<float>(fixed_updating_components.size()) >= removal_threshold)
        BatchEraseDeadComponentsFromMap(fixed_updating_components);
}


void Actor::OnCollisionEnter(const CollisionData &collision)
{
    float dead_components = 0.0f;
    for (auto &actor_component_pair : collision_entering_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnCollisionEnter(collision);
        else
            ++dead_components;
    }
    
    if (dead_components / static_cast<float>(collision_entering_components.size()) >= removal_threshold)
        BatchEraseDeadComponentsFromMap(collision_entering_components);
}


void Actor::OnCollisionExit(const CollisionData &collision)
{
    float dead_components = 0.0f;
    for (auto &actor_component_pair : collision_exiting_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnCollisionExit(collision);
        else
            ++dead_components;
    }
    
    if (dead_components / static_cast<float>(collision_exiting_components.size()) >= removal_threshold)
        BatchEraseDeadComponentsFromMap(collision_exiting_components);
}


void Actor::OnTriggerEnter(const CollisionData &collision)
{
    float dead_components = 0.0f;
    for (auto &actor_component_pair : trigger_entering_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnTriggerEnter(collision);
        else
            ++dead_components;
    }
    
    if (dead_components / static_cast<float>(trigger_entering_components.size()) >= removal_threshold)
        BatchEraseDeadComponentsFromMap(trigger_entering_components);
}


void Actor::OnTriggerExit(const CollisionData &collision)
{
    float dead_components = 0.0f;
    for (auto &actor_component_pair : trigger_exiting_components)
    {
        if (std::shared_ptr<Component> actor_component = actor_component_pair.second.lock())
            actor_component->OnTriggerExit(collision);
        else
            ++dead_components;
    }
    
    if (dead_components / static_cast<float>(trigger_exiting_components.size()) >= removal_threshold)
        BatchEraseDeadComponentsFromMap(trigger_exiting_components);
}


void Actor::OnDestroy()
{
    std::vector<std::shared_ptr<Component>> components_to_destroy_this_frame(std::move(destroying_components));
    
    std::stable_sort(components_to_destroy_this_frame.begin(), components_to_destroy_this_frame.end(), ComponentPtrCompAsc());
    
    for (std::shared_ptr<Component> &component_to_destroy_this_frame : components_to_destroy_this_frame)
    {
        component_to_destroy_this_frame->OnDestroy();
    }
}


sol::object Actor::cppActorGetComponentByKey(const std::string &key)
{
    if (actor_components.count(key) > 0)
        return actor_components[key]->GetComponentRef();
    else
    {
        auto component_to_add_it = std::find_if(components_to_add.begin(), components_to_add.end(), [&key](std::shared_ptr<Component> &component_to_add) {
            return (component_to_add->GetComponentRef()).as<sol::table>()["key"] == key;
        });
        
        if (component_to_add_it != components_to_add.end())
            return (*component_to_add_it)->GetComponentRef();
    }
        
    return sol::make_object(ComponentManager::GetLuaState()->lua_state(), sol::lua_nil);
}


sol::object Actor::cppActorGetComponent(const std::string &type_name)
{
    for (auto &actor_component_pair : actor_components)
    {
        Component &actor_component = *actor_component_pair.second.get();
        if (actor_component.GetComponentType() == type_name)
            return actor_component.GetComponentRef();
    }
    
    auto component_to_add_it = std::find_if(components_to_add.begin(), components_to_add.end(), [&type_name](std::shared_ptr<Component> &component_to_add) {
        return component_to_add->GetComponentType() == type_name;
    });
    
    if (component_to_add_it != components_to_add.end())
        return (*component_to_add_it)->GetComponentRef();
    
    return sol::make_object(ComponentManager::GetLuaState()->lua_state(), sol::lua_nil);
}


sol::table Actor::cppActorGetComponents(const std::string &type_name)
{
    sol::table component_table = ComponentManager::GetLuaState()->create_table();
    
    uint16_t components_found = 0;
    
    for (auto &actor_component_pair : actor_components)
    {
        Component &actor_component = *actor_component_pair.second.get();
        if (actor_component.GetComponentType() == type_name)
        {
            components_found++;
            component_table[components_found] = actor_component.GetComponentRef();
        }
    }
    
    for (auto &component_to_add : components_to_add)
    {
        if (component_to_add->GetComponentType() == type_name)
        {
            components_found++;
            component_table[components_found] = component_to_add->GetComponentRef();
        }
    }
    
    return component_table;
}


void Actor::cppActorRemoveComponent(sol::object component_to_remove)
{
    auto actor_component_it = std::find_if(actor_components.begin(), actor_components.end(), [&component_to_remove](const std::pair<std::string, std::shared_ptr<Component>> &actor_component_pair) {
        return actor_component_pair.second->GetComponentRef() == component_to_remove;
    });
    
    if (actor_component_it != actor_components.end())
    {
        std::shared_ptr<Component> actor_component = actor_component_it->second;
        
        ProcessComponentRemoval(actor_component);
        
        ResetComponentInLifecycleMaps(actor_component);
        
        actor_components.erase(actor_component->GetComponentKey());
        
        return;
    }
    
    
    auto component_to_add_it = std::remove_if(components_to_add.begin(), components_to_add.end(), [&component_to_remove](std::shared_ptr<Component> &component_to_add) {
        return component_to_add->GetComponentRef() == component_to_remove;
    });
    
    if (component_to_add_it != components_to_add.end())
    {
        std::shared_ptr<Component> component_to_add = *component_to_add_it;
        
        ProcessComponentRemoval(component_to_add);
        
        components_to_add.erase(component_to_add_it);
        
        return;
    }
}


void Actor::RemoveAllComponents()
{
    for (auto actor_component_pair : actor_components)
    {
        std::shared_ptr<Component> actor_component = actor_component_pair.second;
        
        ProcessComponentRemoval(actor_component);
        
        ResetComponentInLifecycleMaps(actor_component);
    }
    
    actor_components.clear();
    
    for (std::shared_ptr<Component> component_to_add : components_to_add)
    {
        ProcessComponentRemoval(component_to_add);
    }
    
    components_to_add.clear();
}


void Actor::AddComponents()
{
    for (auto &component_to_add : components_to_add)
    {
        std::string component_key = component_to_add->GetComponentKey();
        
        if (actor_components.try_emplace(component_key, component_to_add).second)
            AddComponentToLifecycleMaps(component_to_add);
    }
    
    components_to_add.clear();
}


void Actor::BatchEraseDeadComponentsFromMap(std::map<std::string, std::weak_ptr<Component>> &flat_map_to_clean)
{
    for (auto component_it = flat_map_to_clean.begin(); component_it != flat_map_to_clean.end();)
    {
        if (std::shared_ptr<Component> component = component_it->second.lock())
            ++component_it;
        else
            component_it = flat_map_to_clean.erase(component_it);
    }
}


void Actor::ProcessComponentRemoval(const std::shared_ptr<Component> &component)
{
    component->SetEnabled(false);
    
    if (component->HasOnDestroy())
    {
        destroying_components.emplace_back(component);
        
        SceneManager::current_scene.actors_destroying_components.emplace(self_reference.lock());
    }
}


void Actor::AddActorToSceneStartingMap() const { SceneManager::current_scene.starting_actors_to_add.emplace_back(self_reference.lock()); }


// TODO: Convert Actors and Components to full factory pattern, post hw-8

// TODO: Add exporting
// TODO: Add webasm export support
// TODO: Feature parity with LazyFoo
// TODO: Add full dualsense support
// TODO: Extensive Tweening support
// TODO: Data-orient dstrect calculations
// TODO: Replace RapidJSON
// TODO: Fix your timestep!
// TODO: OpenGL shaders *maybe*
