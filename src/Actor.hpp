//
//  Actor.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 1/29/24.
//

#ifndef Actor_hpp
#define Actor_hpp

#include "ComponentManager.hpp"
#include "document.h"
#include "Utilities.hpp"

#include <functional>
#include <list>
#include <map>
#include <optional>
#include <queue>
#include <string>
#include <set>

struct CollisionData;

class Actor : public std::enable_shared_from_this<Actor>
{
    friend class Scene;
    
public:
    
    
    Actor();
    
    
    Actor(const Actor &other);
    
    
    void SetSelfReference(const std::shared_ptr<Actor> &actor_ptr);
    
    
    std::shared_ptr<Actor> GetSharedPointer();
    
    
    void UpdateActorWithJSON(const rapidjson::Value &actor_data);
    
    
    void AddComponents();
    
    
    void OnStart();
    
    
    void OnUpdate();
    
    
    void OnLateUpdate();
    
    
    void OnFixedUpdate();
    
    
    void OnCollisionEnter(const CollisionData &collision);
    
    
    void OnCollisionExit(const CollisionData &collision);
    
    
    void OnTriggerEnter(const CollisionData &collision);
    
    
    void OnTriggerExit(const CollisionData &collision);
    
    
    void OnDestroy();
    
    
    std::string cppActorGetName();
    
    
    uint32_t cppActorGetID();
    
    
    sol::object cppActorGetComponentByKey(const std::string &key);
    
    
    sol::object cppActorGetComponent(const std::string &type_name);
    
    
    sol::table cppActorGetComponents(const std::string &type_name);
    
    
    sol::object cppActorAddComponent(const std::string &type_name);
    
    
    void cppActorRemoveComponent(sol::object component_to_remove);
    
    
    void RemoveAllComponents();
    
    
    struct less {
        bool operator() (const Actor* lhs, const Actor* rhs) const { return lhs->uuid < rhs->uuid; }
        bool operator() (const std::shared_ptr<Actor> lhs, const std::shared_ptr<Actor> rhs) const { return lhs->uuid < rhs->uuid; }
    };

    struct more {
        bool operator() (const Actor* lhs, const Actor* rhs) const { return lhs->uuid > rhs->uuid; }
        bool operator() (const std::shared_ptr<Actor> lhs, const std::shared_ptr<Actor> rhs) const { return lhs->uuid > rhs->uuid; }
    };
    
    
    std::map<std::string, std::shared_ptr<Component>> actor_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> starting_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> updating_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> late_updating_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> fixed_updating_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> collision_entering_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> collision_exiting_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> trigger_entering_components;
    
    
    std::map<std::string, std::weak_ptr<Component>> trigger_exiting_components;
    
    
    std::vector<std::shared_ptr<Component>> destroying_components;
    
    
    std::vector<std::shared_ptr<Component>> components_to_add;
    
    
    std::string name;
    
    
    uint32_t uuid;

    
private:
    
    
    std::shared_ptr<Component> QueueComponentToAdd(std::shared_ptr<Component> &component);
    
    
    void AddComponentToLifecycleMaps(const std::shared_ptr<Component> &actor_component);
    
    
    void ResetComponentInLifecycleMaps(const std::shared_ptr<Component> &actor_component);
    
    
    void AddActorToSceneStartingMap() const;
    
    
    void BatchEraseDeadComponentsFromMap(std::map<std::string, std::weak_ptr<Component>> &flat_map_to_clean);
    
    
    void ProcessComponentRemoval(const std::shared_ptr<Component> &component);
    
    
    std::weak_ptr<Actor> self_reference;
    
    
    static inline uint32_t next_uuid = 0;
    
    
    static inline uint32_t runtime_components_added;
    
    
    static inline const float removal_threshold = 0.3f;
};


inline Actor::Actor() : uuid(next_uuid++)               {}


inline std::shared_ptr<Actor> Actor::GetSharedPointer() { return shared_from_this(); }


inline std::string Actor::cppActorGetName()             { return name; }


inline uint32_t Actor::cppActorGetID()                       { return uuid; }


inline sol::object Actor::cppActorAddComponent(const std::string &type_name)
{
    std::shared_ptr<Component> component = ComponentManager::MakeComponent(type_name);

    QueueComponentToAdd(component)->SetComponentKey("r" + std::to_string(runtime_components_added++));

    AddActorToSceneStartingMap();
    
    return component->GetComponentRef();
}


inline std::shared_ptr<Component> Actor::QueueComponentToAdd(std::shared_ptr<Component> &component)
{
    components_to_add.emplace_back(component);
    
    component->SetActor(self_reference);
    
    return component;
}


inline void Actor::AddComponentToLifecycleMaps(const std::shared_ptr<Component> &actor_component)
{
    std::string component_key = actor_component->GetComponentKey();
    
    starting_components[component_key] = actor_component;
    
    if (actor_component->HasOnUpdate())
        updating_components[component_key] = actor_component;
    
    if (actor_component->HasOnLateUpdate())
        late_updating_components[component_key] = actor_component;
    
    if (actor_component->HasOnFixedUpdate())
        fixed_updating_components[component_key] = actor_component;
    
    if (actor_component->HasOnCollisionEnter())
        collision_entering_components[component_key] = actor_component;
    
    if (actor_component->HasOnCollisionExit())
        collision_exiting_components[component_key] = actor_component;
    
    if (actor_component->HasOnTriggerEnter())
        trigger_entering_components[component_key] = actor_component;
    
    if (actor_component->HasOnTriggerExit())
        trigger_exiting_components[component_key] = actor_component;
}

inline void Actor::ResetComponentInLifecycleMaps(const std::shared_ptr<Component> &actor_component)
{
    if (actor_component->HasOnStart())
        starting_components[actor_component->GetComponentKey()].reset();
    
    if (actor_component->HasOnUpdate())
        updating_components[actor_component->GetComponentKey()].reset();
    
    if (actor_component->HasOnLateUpdate())
        late_updating_components[actor_component->GetComponentKey()].reset();
    
    if (actor_component->HasOnFixedUpdate())
        fixed_updating_components[actor_component->GetComponentKey()].reset();
    
    if (actor_component->HasOnCollisionEnter())
        collision_entering_components[actor_component->GetComponentKey()].reset();
    
    if (actor_component->HasOnCollisionExit())
        collision_exiting_components[actor_component->GetComponentKey()].reset();
    
    if (actor_component->HasOnTriggerEnter())
        trigger_entering_components[actor_component->GetComponentKey()].reset();
    
    if (actor_component->HasOnTriggerExit())
        trigger_exiting_components[actor_component->GetComponentKey()].reset();
}


#endif /* Actor_hpp */
