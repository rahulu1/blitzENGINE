//
//  Rigidbody.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/20/24.
//

#ifndef Rigidbody_hpp
#define Rigidbody_hpp

#include "CollisionManager.hpp"
#include "NativeComponent.hpp"
#include "TweenManager.hpp"
#include "sol/sol.hpp"

#include <stdio.h>
#include <string>


class Rigidbody : public NativeComponent {
public:
    
    
    Rigidbody();
    
    
    Rigidbody(const Rigidbody &other);
    
    
    void UpdateComponentWithJSON(const rapidjson::Value &component_json) override;
    
    
    std::shared_ptr<Component> Clone() const override;
    
    
    std::shared_ptr<Component> GetSharedPointer() override;
    
    
    void OnStart() override;
    
    
    void OnUpdate() override;
    
    
    void OnLateUpdate() override;
    
    
    void OnFixedUpdate() override;
    
    
    void OnCollisionEnter(const CollisionData &collision) override;
    
    
    void OnCollisionExit(const CollisionData &collision) override;
    
    
    void OnTriggerEnter(const CollisionData &collision) override;
    
    
    void OnTriggerExit(const CollisionData &collision) override;
    
    
    void OnDestroy() override;
    
    
    ITween* GOMove(b2Vec2 end, float duration);
    
    
    void AddForce(const b2Vec2 &vec2);
    
    
    void SetVelocity(const b2Vec2 &vec2);
    
    
    b2Vec2 GetVelocity() const;
    
    
    static b2World* GetWorld();
    
    
    void SetBodyType(const std::string &body_type);
    

    std::string GetBodyType() const;
    
    
    void SetColliderType(const std::string &collider_type);
    

    std::string GetColliderType() const;
    
    
    void SetTriggerType(const std::string &trigger_type);
    

    std::string GetTriggerType() const;
    
    
    void SetPosition(const b2Vec2 &vec2);
    
    
    void MovePosition(const b2Vec2 &vec2);
    
    
    void SetPositionX(float x);
    
    
    void SetPositionY(float y);
    
    
    b2Vec2 GetPosition() const;
    
    
    float GetPositionX() const;
    
    
    float GetPositionY() const;
    
    
    void SetRotation(float rotation);
    
    
    float GetRotation() const;
    
    
    void SetAngularVelocity(float degrees_clockwise);
    
    
    float GetAngularVelocity() const;
    
    
    void SetGravityScale(float gravity_scale);
    
    
    float GetGravityScale() const;
    
    
    void SetUpDirection(b2Vec2 &vec2);
    
    
    b2Vec2 GetUpDirection();
    
    
    void SetRightDirection(b2Vec2 &vec2);
    
    
    b2Vec2 GetRightDirection();
    
    
    void SetDensity(float density);
    
    
    float GetDensity() const;
    
    
    void SetLinearFriction(float friction);
    
    
    float GetLinearFriction() const;
    
    
    void SetAngularFriction(float angular_friction);
    
    
    float GetAngularFriction() const;
    
    
    void SetPrecise(bool precise);
    
    
    float GetMass();
    
    
    bool IsPrecise() const;
    
    
    void SetCollider(bool has_collider);
    
    
    bool HasCollider() const;
    
    
    void SetTrigger(bool has_trigger);
    
    
    bool HasTrigger() const;
    
private:
    
    
    enum
    {
        e_preciseFlag   = 0x0001,
        e_colliderFlag  = 0x0002,
        e_triggerFlag   = 0x0004
    };
    
    
    std::string _body_type = "dynamic";
    
    
    std::string _collider_type = "box";
    
    
    std::string _trigger_type = "box";
    
    
    b2Vec2 _start_force = b2Vec2(0.0f, 0.0f);
    
    
    b2Vec2 _velocity = b2Vec2(0.0f, 0.0f);
    
    
    b2Vec2 _position = b2Vec2(0.0f, 0.0f);
    
    
    static inline std::unique_ptr<CollisionManager> collision_manager;
    
    
    static inline b2World* world;
    
    
    b2Body* body = nullptr;
    
    
    float _rotation = 0.0f;
    
    
    float _angular_velocity = 0.0f;
    
    
    float _gravity_scale = 1.0f;
    
    
    float _density = 1.0f;
    
    
    float _angular_friction = 0.3f;
    
    
    float _friction = 0.3f;
    
    
    float _bounciness = 0.3f;
    
    
    float _width = 1.0f;
    
    
    float _height = 1.0f;
    
    
    float _radius = 0.5f;
    
    
    float _trigger_width = 1.0f;
    
    
    float _trigger_height = 1.0f;
    
    
    float _trigger_radius = 0.5f;
    
    
    float _mass = 1.0f;
    
    
    uint16 _flags = (e_preciseFlag | e_colliderFlag | e_triggerFlag);
};


inline std::shared_ptr<Component> Rigidbody::Clone() const { return std::make_shared<Rigidbody>(*this); }


inline std::shared_ptr<Component> Rigidbody::GetSharedPointer() { return shared_from_this(); }


inline void Rigidbody::OnUpdate() {}


inline void Rigidbody::OnLateUpdate() {}


inline void Rigidbody::OnFixedUpdate()
{
    if (body)
    {
        body->SetGravityScale(_gravity_scale);
        body->SetLinearDamping(_friction);
    }
}


inline ITween* Rigidbody::GOMove(b2Vec2 end, float duration)
{
    std::function<b2Vec2()> getter = [this](){ return this->GetPosition(); };
    std::function<void(b2Vec2 x)> setter = [this](b2Vec2 new_pos){ this->MovePosition(new_pos); };
    return TweenManager::GOTo(getter, setter, GetPosition(), end, duration);
}


inline void Rigidbody::OnCollisionEnter(const CollisionData &collision) {}


inline void Rigidbody::OnCollisionExit(const CollisionData &collision) {}


inline void Rigidbody::OnTriggerEnter(const CollisionData &collision) {}


inline void Rigidbody::OnTriggerExit(const CollisionData &collision) {}


inline void Rigidbody::AddForce(const b2Vec2 &vec2)
{
    if (body)
        body->ApplyForceToCenter(vec2, true);
    else
        _start_force += vec2;
}


inline void Rigidbody::SetVelocity(const b2Vec2 &vec2)
{
    if (body)
        body->SetLinearVelocity(vec2);
    else
        _velocity = vec2;
}


inline b2Vec2 Rigidbody::GetVelocity() const
{
    if (body)
        return body->GetLinearVelocity();
    else
        return _velocity;
}


inline b2World* Rigidbody::GetWorld() { return world; }


inline void Rigidbody::SetBodyType(const std::string &body_type)
{
    if (body)
        return;
    else
        _body_type = body_type;
}


inline std::string Rigidbody::GetBodyType() const { return _body_type; }


inline void Rigidbody::SetColliderType(const std::string &collider_type)
{
    if (body)
        return;
    else
        _collider_type = collider_type;
}


inline std::string Rigidbody::GetColliderType() const { return _collider_type; }


inline void Rigidbody::SetTriggerType(const std::string &trigger_type)
{
    if (body)
        return;
    else
        _trigger_type = trigger_type;
}


inline std::string Rigidbody::GetTriggerType() const { return _trigger_type; }


inline void Rigidbody::SetPosition(const b2Vec2 &vec2)
{
    if (body)
        body->SetTransform(vec2, body->GetAngle());
    else
        _position = vec2;
}


inline void Rigidbody::SetPositionX(float x)
{
    if (body)
        return;
    else
        _position.x = x;
}


inline void Rigidbody::SetPositionY(float y)
{
    if (body)
        return;
    else
        _position.y = y;
}


inline b2Vec2 Rigidbody::GetPosition() const
{
    if (body)
        return body->GetPosition();
    else
        return _position;
}


inline float Rigidbody::GetPositionX() const { return GetPosition().x; }


inline float Rigidbody::GetPositionY() const { return GetPosition().y; }


inline void Rigidbody::SetRotation(float rotation)
{
    if (body)
        body->SetTransform(body->GetPosition(), rotation * (b2_pi / 180.0f));
    else
        _rotation = rotation * (b2_pi / 180.0f);
}


inline float Rigidbody::GetRotation() const
{
    if (body)
        return body->GetAngle() * (180.0f / b2_pi);
    else
        return _rotation * (180.0f / b2_pi);
}


inline void Rigidbody::SetAngularVelocity(float degrees_clockwise)
{
    if (body)
        body->SetAngularVelocity(degrees_clockwise * (b2_pi / 180.0f));
    else
        _angular_velocity = degrees_clockwise * (b2_pi / 180.0f);
}


inline float Rigidbody::GetAngularVelocity() const
{
    if (body)
        return body->GetAngularVelocity() * (180.0f / b2_pi);
    else
        return _angular_velocity * (180.0f / b2_pi);
}


inline void Rigidbody::SetUpDirection(b2Vec2 &vec2)
{
    vec2.Normalize();
    
    if (body)
        body->SetTransform(body->GetPosition(), glm::atan(vec2.x, -vec2.y));
    else
        _rotation = glm::atan(vec2.x, -vec2.y);
}


inline b2Vec2 Rigidbody::GetUpDirection()
{
    if (body)
        _rotation = body->GetAngle();
    
    b2Vec2 up_vector = b2Vec2(glm::sin(_rotation), -glm::cos(_rotation));
    
    up_vector.Normalize();
    return up_vector;
}


inline void Rigidbody::SetRightDirection(b2Vec2 &vec2)
{
    vec2.Normalize();
    
    if (body)
        body->SetTransform(body->GetPosition(), glm::atan(vec2.x, -vec2.y) - (b2_pi / 2.0f));
    else
        _rotation = glm::atan(vec2.x, -vec2.y) - (b2_pi / 2.0f);
}


inline b2Vec2 Rigidbody::GetRightDirection()
{
    if (body)
        _rotation = body->GetAngle();
    
    b2Vec2 right_vector = b2Vec2(glm::cos(_rotation), glm::sin(_rotation));
    
    right_vector.Normalize();
    return right_vector;
}


inline void Rigidbody::SetGravityScale(float gravity_scale)
{
    if (body)
        body->SetGravityScale(gravity_scale);

    _gravity_scale = gravity_scale;
}


inline float Rigidbody::GetGravityScale() const { return _gravity_scale; }


inline void Rigidbody::SetDensity(float density)
{
    if (body)
        return;
    
    _density = density;
}


inline float Rigidbody::GetDensity() const { return _density; }


inline void Rigidbody::SetLinearFriction(float friction)
{
    if (body)
        body->SetLinearDamping(friction);
    
    _friction = friction;
}


inline float Rigidbody::GetLinearFriction()  const { return _friction; }


inline void Rigidbody::SetAngularFriction(float angular_friction)
{
    if (body)
        body->SetAngularDamping(angular_friction);
    
    _angular_friction = angular_friction;
}


inline float Rigidbody::GetAngularFriction()  const { return _angular_friction; }


inline void Rigidbody::SetPrecise(bool precise)
{
    if (body)
        return;
    
    if (precise)
        _flags |= e_preciseFlag;
    else
        _flags &= ~e_preciseFlag;
}


inline bool Rigidbody::IsPrecise() const
{
    if (body)
        return body->IsBullet();
    
    return (_flags & e_preciseFlag) == e_preciseFlag;
}


inline void Rigidbody::SetCollider(bool has_collider)
{
    if (has_collider)
        _flags |= e_colliderFlag;
    else
        _flags &= ~e_colliderFlag;
}


inline bool Rigidbody::HasCollider() const { return (_flags & e_colliderFlag) == e_colliderFlag; }


inline void Rigidbody::SetTrigger(bool has_trigger)
{
    if (has_trigger)
        _flags |= e_triggerFlag;
    else
        _flags &= ~e_triggerFlag;
}

inline bool Rigidbody::HasTrigger() const { return (_flags & e_triggerFlag) == e_triggerFlag; }


struct HitResult {
    
    b2Vec2 point;
    
    b2Vec2 normal;
    
    Actor* actor;
    
    float fraction;
    
    bool is_trigger;
    
    bool operator()(const HitResult &lhs, const HitResult &rhs) const { return lhs.fraction < rhs.fraction; }
};


struct PhysicsRaycastAllCallback : b2RayCastCallback {
    
    std::vector<HitResult> hit_results;
    
    float ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction) override
    {
        HitResult hit_result;
        
        hit_result.point = point;
        hit_result.normal = normal;
        hit_result.actor = reinterpret_cast<Actor*>(fixture->GetUserData().pointer);
        hit_result.fraction = fraction;
        hit_result.is_trigger = fixture->IsSensor();
        
        if (!hit_result.actor)
            return -1;
        
        hit_results.emplace_back(hit_result);
        return 1;
    }
};



static inline sol::object cppPhysicsRaycast(b2Vec2 position, b2Vec2 direction, float distance)
{
    if (Rigidbody::GetWorld())
    {
        PhysicsRaycastAllCallback raycast_all_callback;
        
        direction.Normalize();
        
        b2Vec2 end_position = position + (distance * direction);
        
        Rigidbody::GetWorld()->RayCast(&raycast_all_callback, position, end_position);
                
        
        if (!raycast_all_callback.hit_results.empty())
        {
            std::stable_sort(raycast_all_callback.hit_results.begin(), raycast_all_callback.hit_results.end(), HitResult());
            return sol::make_object(ComponentManager::GetLuaState()->lua_state(),raycast_all_callback.hit_results.front());
        }
            
    }
    
    return sol::make_object(ComponentManager::GetLuaState()->lua_state(), sol::lua_nil);
}


static inline sol::table cppPhysicsRaycastAll(b2Vec2 position, b2Vec2 direction, float distance)
{
    sol::table hit_result_table = ComponentManager::GetLuaState()->create_table();
    
    if (Rigidbody::GetWorld())
    {
        int hits = 0;
        
        PhysicsRaycastAllCallback raycast_all_callback;
        
        direction.Normalize();
        
        b2Vec2 end_position = position + (distance * direction);
        
        Rigidbody::GetWorld()->RayCast(&raycast_all_callback, position, end_position);

        
        if (!raycast_all_callback.hit_results.empty())
        {
            std::stable_sort(raycast_all_callback.hit_results.begin(), raycast_all_callback.hit_results.end(), HitResult());
            
            for (HitResult hit_result : raycast_all_callback.hit_results)
            {
                hit_result_table[++hits] = sol::make_object(ComponentManager::GetLuaState()->lua_state(), hit_result);
            }
        }
    }
    
    return hit_result_table;
}

#endif /* Rigidbody_hpp */
