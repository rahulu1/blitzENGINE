//
//  CollisionManager.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/30/24.
//

#include "CollisionManager.hpp"

void CollisionManager::BeginContact(b2Contact *contact)
{
    b2Fixture* fixture_a = contact->GetFixtureA();
    b2Fixture* fixture_b = contact->GetFixtureB();
    
    if (fixture_a->IsSensor() != fixture_b->IsSensor())
        return;
    
    Actor* actor_a = reinterpret_cast<Actor*>(fixture_a->GetUserData().pointer);
    Actor* actor_b = reinterpret_cast<Actor*>(fixture_b->GetUserData().pointer);
    
    b2WorldManifold world_manifold;
    contact->GetWorldManifold(&world_manifold);
    
    
    if (actor_a && actor_b)
    {
        CollisionData collision_a;
        CollisionData collision_b;
        
        collision_a.other = actor_b;
        collision_b.other = actor_a;
        
        collision_a.relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();
        collision_b.relative_velocity = collision_a.relative_velocity;
        
        if (!fixture_a->IsSensor())
        {
            collision_a.point = world_manifold.points[0];
            collision_b.point = world_manifold.points[0];
            
            collision_a.normal = world_manifold.normal;
            collision_b.normal = world_manifold.normal;
            
            callbacks_to_process.emplace_back([actor_a, collision_a] { actor_a->OnCollisionEnter(collision_a); });
            callbacks_to_process.emplace_back([actor_b, collision_b] { actor_b->OnCollisionEnter(collision_b); });
        }
        else
        {
            callbacks_to_process.emplace_back([actor_a, collision_a] { actor_a->OnTriggerEnter(collision_a); });
            callbacks_to_process.emplace_back([actor_b, collision_b] { actor_b->OnTriggerEnter(collision_b); });
        }
    }
}

void CollisionManager::EndContact(b2Contact *contact)
{
    b2Fixture* fixture_a = contact->GetFixtureA();
    b2Fixture* fixture_b = contact->GetFixtureB();
    
    if (fixture_a->IsSensor() != fixture_b->IsSensor())
        return;
    
    Actor* actor_a = reinterpret_cast<Actor*>(fixture_a->GetUserData().pointer);
    Actor* actor_b = reinterpret_cast<Actor*>(fixture_b->GetUserData().pointer);
    
    b2WorldManifold world_manifold;
    contact->GetWorldManifold(&world_manifold);
    
    
    if (actor_a && actor_b)
    {
        CollisionData collision_a;
        CollisionData collision_b;
        
        
        collision_a.other = actor_b;
        collision_b.other = actor_a;
        
        collision_a.relative_velocity = fixture_a->GetBody()->GetLinearVelocity() - fixture_b->GetBody()->GetLinearVelocity();
        collision_b.relative_velocity = collision_a.relative_velocity;
        
        
        if (!fixture_a->IsSensor())
        {
            callbacks_to_process.emplace_back([actor_a, collision_a] { actor_a->OnCollisionExit(collision_a); });
            callbacks_to_process.emplace_back([actor_b, collision_b] { actor_b->OnCollisionExit(collision_b); });
        }
        else
        {
            callbacks_to_process.emplace_back([actor_a, collision_a] { actor_a->OnTriggerExit(collision_a); });
            callbacks_to_process.emplace_back([actor_b, collision_b] { actor_b->OnTriggerExit(collision_b); });
        }
    }
}


void CollisionManager::ProcessContactCallbacks()
{
    for (const std::function<void()> &callback_to_process : callbacks_to_process) { callback_to_process(); }
    
    callbacks_to_process.clear();
}
