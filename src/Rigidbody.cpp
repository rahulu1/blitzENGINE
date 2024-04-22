//
//  Rigidbody.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/20/24.
//

#include "Rigidbody.hpp"
#include "Engine.h"

Rigidbody::Rigidbody()
{
    if (!world)
    {
        b2Vec2 gravity(0.0f, 9.8f);
        world = new b2World(gravity);
        
        collision_manager = std::make_unique<CollisionManager>();
        world->SetContactListener(collision_manager.get());
    }
    
    type = "Rigidbody";
    
    component_ref = sol::make_object(ComponentManager::GetLuaState()->lua_state(), this);
    
    has_on_fixed_update = true;
    has_on_destroy = true;
}


Rigidbody::Rigidbody(const Rigidbody &other)
    :   NativeComponent::NativeComponent(other),
        _body_type(other._body_type),
        _collider_type(other._collider_type),
        _trigger_type(other._trigger_type),
        _position(other._position),
        _rotation(other._rotation),
        _gravity_scale(other._gravity_scale),
        _density(other._density),
        _angular_friction(other._angular_friction),
        _friction(other._friction),
        _bounciness(other._bounciness),
        _width(other._width),
        _height(other._height),
        _radius(other._radius),
        _trigger_width(other._trigger_width),
        _trigger_height(other._trigger_height),
        _trigger_radius(other._trigger_radius),
        _flags(other._flags)
{
    component_ref = sol::make_object(ComponentManager::GetLuaState()->lua_state(), this);
}


void Rigidbody::UpdateComponentWithJSON(const rapidjson::Value &component_json)
{
    if (component_json.HasMember("body_type") && component_json["body_type"].IsString())
        SetBodyType(component_json["body_type"].GetString());
    
    if (component_json.HasMember("collider_type") && component_json["collider_type"].IsString())
        SetColliderType(component_json["collider_type"].GetString());
    
    if (component_json.HasMember("trigger_type") && component_json["trigger_type"].IsString())
        SetTriggerType(component_json["trigger_type"].GetString());
    
    if (component_json.HasMember("x") && component_json["x"].IsNumber())
        SetPositionX(component_json["x"].GetFloat());
    
    if (component_json.HasMember("y") && component_json["y"].IsNumber())
        SetPositionY(component_json["y"].GetFloat());
    
    if (component_json.HasMember("gravity_scale") && component_json["gravity_scale"].IsNumber())
        SetGravityScale(component_json["gravity_scale"].GetFloat());
    
    if (component_json.HasMember("density") && component_json["density"].IsNumber())
        SetDensity(component_json["density"].GetFloat());
    
    if (component_json.HasMember("angular_friction") && component_json["angular_friction"].IsNumber())
        SetAngularFriction(component_json["angular_friction"].GetFloat());
    
    if (component_json.HasMember("rotation") && component_json["rotation"].IsNumber())
        SetRotation(component_json["rotation"].GetFloat());
    
    if (component_json.HasMember("width") && component_json["width"].IsNumber())
        _width = component_json["width"].GetFloat();
    
    if (component_json.HasMember("height") && component_json["height"].IsNumber())
        _height = component_json["height"].GetFloat();
    
    if (component_json.HasMember("radius") && component_json["radius"].IsNumber())
        _radius = component_json["radius"].GetFloat();
    
    if (component_json.HasMember("trigger_width") && component_json["trigger_width"].IsNumber())
        _trigger_width = component_json["trigger_width"].GetFloat();
    
    if (component_json.HasMember("trigger_height") && component_json["trigger_height"].IsNumber())
        _trigger_height = component_json["trigger_height"].GetFloat();
    
    if (component_json.HasMember("trigger_radius") && component_json["trigger_radius"].IsNumber())
        _trigger_radius = component_json["trigger_radius"].GetFloat();
    
    if (component_json.HasMember("friction") && component_json["friction"].IsNumber())
        _friction = component_json["friction"].GetFloat();
    
    if (component_json.HasMember("bounciness") && component_json["bounciness"].IsNumber())
        _bounciness = component_json["bounciness"].GetFloat();
    
    if (component_json.HasMember("precise") && component_json["precise"].IsBool())
        _flags = component_json["precise"].GetBool() ? (_flags | e_preciseFlag) : (_flags & ~e_preciseFlag);
    
    if (component_json.HasMember("has_collider") && component_json["has_collider"].IsBool())
        _flags = component_json["has_collider"].GetBool() ? (_flags | e_colliderFlag) : (_flags & ~e_colliderFlag);
    
    if (component_json.HasMember("has_trigger") && component_json["has_trigger"].IsBool())
        _flags = component_json["has_trigger"].GetBool() ? (_flags | e_triggerFlag) : (_flags & ~e_triggerFlag);
}


void Rigidbody::OnStart()
{
    b2BodyDef body_def;
    
    if (_body_type == "dynamic")
        body_def.type = b2_dynamicBody;
    else if (_body_type == "static")
        body_def.type = b2_staticBody;
    else if (_body_type == "kinematic")
        body_def.type = b2_kinematicBody;
    else
        ErrorExit("Invalid Body Type");
    
    body_def.position = _position;
    body_def.angle = _rotation;
    body_def.bullet = IsPrecise();
    body_def.angularDamping = _angular_friction;
    body_def.gravityScale = _gravity_scale;
    
    body = world->CreateBody(&body_def);
    
    if (!HasCollider() && !HasTrigger())
    {
        b2PolygonShape phantom_shape;
        phantom_shape.SetAsBox(_width * 0.5f, _height * 0.5f);
        
        b2FixtureDef phantom_fixture_def;
        phantom_fixture_def.shape = &phantom_shape;
        phantom_fixture_def.density = _density;
        
        phantom_fixture_def.isSensor = true;
        body->CreateFixture(&phantom_fixture_def);
        
        return;
    }
    else
    {
        
        if(HasCollider())
        {
            b2FixtureDef fixture_def;
            
            if (std::shared_ptr<Actor> actor_sp = actor.lock())
            {
                body_def.userData.pointer = reinterpret_cast<uintptr_t>(actor_sp.get());
                fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor_sp.get());
            }
            
            std::shared_ptr<b2Shape> shape;
            
            if (_collider_type == "box")
            {
                shape = std::make_unique<b2PolygonShape>();
                std::dynamic_pointer_cast<b2PolygonShape>(shape)->SetAsBox(_width * 0.5f, _height * 0.5f);
                
                fixture_def.shape = std::dynamic_pointer_cast<b2PolygonShape>(shape).get();
            }
            else if (_collider_type == "circle")
            {
                shape = std::make_unique<b2CircleShape>();
                std::dynamic_pointer_cast<b2CircleShape>(shape)->m_radius = _radius;
                
                fixture_def.shape = std::dynamic_pointer_cast<b2CircleShape>(shape).get();
            }
            else
                ErrorExit("Invalid Collider Shape");
            
            
            fixture_def.density = _density;
            fixture_def.friction = _friction;
            fixture_def.restitution = _bounciness;
            
            fixture_def.isSensor = false;
            
            body->CreateFixture(&fixture_def);
        }
        
        if (HasTrigger())
        {
            b2FixtureDef fixture_def;
            
            if (std::shared_ptr<Actor> actor_sp = actor.lock())
            {
                body_def.userData.pointer = reinterpret_cast<uintptr_t>(actor_sp.get());
                fixture_def.userData.pointer = reinterpret_cast<uintptr_t>(actor_sp.get());
            }
            
            std::shared_ptr<b2Shape> shape;
            
            if (_trigger_type == "box")
            {
                shape = std::make_unique<b2PolygonShape>();
                std::dynamic_pointer_cast<b2PolygonShape>(shape)->SetAsBox(_trigger_width * 0.5f, _trigger_height * 0.5f);
                
                fixture_def.shape = std::dynamic_pointer_cast<b2PolygonShape>(shape).get();
            }
            else if (_trigger_type == "circle")
            {
                shape = std::make_unique<b2CircleShape>();
                std::dynamic_pointer_cast<b2CircleShape>(shape)->m_radius = _trigger_radius;
                
                fixture_def.shape = std::dynamic_pointer_cast<b2CircleShape>(shape).get();
            }
            else
                ErrorExit("Invalid Trigger Shape");
            
            
            fixture_def.density = _density;
            fixture_def.friction = _friction;
            fixture_def.restitution = _bounciness;
            
            fixture_def.isSensor = true;
            
            body->CreateFixture(&fixture_def);
        }
    }
    
    body->SetLinearVelocity(_velocity);
    body->SetAngularVelocity(_angular_velocity);
    body->ApplyForceToCenter(_start_force, true);
}


void Rigidbody::OnDestroy() { world->DestroyBody(body); }


void Rigidbody::MovePosition(const b2Vec2 &vec2)
{
    if (body)
    {
        b2Vec2 travel_vector = vec2.operator_sub(body->GetPosition());
        float fixed_delta_time = Engine::GetFixedDeltaTime();
        travel_vector.x /= fixed_delta_time;
        travel_vector.y /= fixed_delta_time;
        body->SetGravityScale(0.0f);
        body->SetLinearDamping(0.0f);
        body->SetLinearVelocity(travel_vector);
    }
    else
        _position = vec2;
}


float Rigidbody::GetMass()
{
    if (body)
        return body->GetMass();
    else
    {
        float mass = 0.0f;
        if (HasCollider())
        {
            if (_collider_type == "box")
                mass += _density * _width * _height;
            else
                mass += _density * b2_pi * _radius * _radius;
        }
        
        if (HasTrigger())
        {
            if (_trigger_type == "box")
                mass += _density * _trigger_width * _trigger_height;
            else
                mass += _density * b2_pi * _trigger_radius * _trigger_radius;
        }
        
        if (!HasTrigger() && !HasCollider())
            mass = 1.0f;
        
        return mass;
    }
}
