//
//  ComponentManager.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/6/24.
//

#define SOL_ALL_SAFETIES_ON 1

#include "ComponentManager.hpp"

#include "AudioManager.hpp"
#include "Engine.h"
#include "EventBus.hpp"
#include "Input.hpp"
#include "LuaComponent.hpp"
#include "Rigidbody.hpp"
#include "TextManager.hpp"
#include "TweenManager.hpp"

#include <any>
#include <functional>

namespace fs =  std::filesystem;


std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>> __native_component_factory = {
    {"Rigidbody", []() -> std::shared_ptr<Component> { return std::make_shared<Rigidbody>(); }}
};


void ComponentManager::Init()
{
    L = sol::state();
    L.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
    
    L.new_usertype<Actor>("Actor",
    "GetName", sol::c_call<decltype(&Actor::cppActorGetName), &Actor::cppActorGetName>,
    "GetID", sol::c_call<decltype(&Actor::cppActorGetID), &Actor::cppActorGetID>,
    "GetComponentByKey", sol::c_call<decltype(&Actor::cppActorGetComponentByKey), &Actor::cppActorGetComponentByKey>,
    "GetComponent", sol::c_call<decltype(&Actor::cppActorGetComponent), &Actor::cppActorGetComponent>,
    "GetComponents", sol::c_call<decltype(&Actor::cppActorGetComponents), &Actor::cppActorGetComponents>,
    "AddComponent", sol::c_call<decltype(&Actor::cppActorAddComponent), &Actor::cppActorAddComponent>,
    "RemoveComponent", sol::c_call<decltype(&Actor::cppActorRemoveComponent), &Actor::cppActorRemoveComponent>);
    
    
    L.new_usertype<CollisionData>("collision",
    "other", &CollisionData::other,
    "point", &CollisionData::point,
    "relative_velocity", &CollisionData::relative_velocity,
    "normal", &CollisionData::normal);
    
    
    L.new_usertype<Component>("Component",
    sol::base_classes, sol::bases<std::enable_shared_from_this<Component>>(),
    "key", sol::c_call<decltype(&Component::GetComponentKey), &Component::GetComponentKey>);
    
    
    L.new_usertype<HitResult>("HitResult",
    "point", &HitResult::point,
    "normal", &HitResult::normal,
    "actor", &HitResult::actor,
    "is_trigger", &HitResult::is_trigger);
    
    
    L.new_usertype<NativeComponent>("NativeComponent",
    sol::base_classes, sol::bases<std::enable_shared_from_this<Component>, Component>(),
    "actor", sol::property(&NativeComponent::GetActor));

    
    L.new_usertype<Rigidbody>("Rigidbody",
    sol::base_classes, sol::bases<std::enable_shared_from_this<Component>, Component, NativeComponent>(),
    "x", sol::property(&Rigidbody::GetPositionX, &Rigidbody::SetPositionX),
    "y", sol::property(&Rigidbody::GetPositionY, &Rigidbody::SetPositionY),
    "body_type", sol::property(&Rigidbody::GetBodyType, &Rigidbody::SetBodyType),
    "precise", sol::property(&Rigidbody::IsPrecise, &Rigidbody::SetPrecise),
    "gravity_scale", sol::property(&Rigidbody::GetGravityScale, &Rigidbody::SetGravityScale),
    "density", sol::property(&Rigidbody::GetDensity, &Rigidbody::SetDensity),
    "angular_friction", sol::property(&Rigidbody::GetAngularFriction, &Rigidbody::SetAngularFriction),
    "rotation", sol::property(&Rigidbody::GetRotation, &Rigidbody::SetRotation),
    "AddForce", sol::c_call<decltype(&Rigidbody::AddForce), &Rigidbody::AddForce>,
    "GOMove", sol::c_call<decltype(&Rigidbody::GOMove), &Rigidbody::GOMove>,
    "MovePosition", sol::c_call<decltype(&Rigidbody::MovePosition), &Rigidbody::MovePosition>,
    "SetPosition", sol::c_call<decltype(&Rigidbody::SetPosition), &Rigidbody::SetPosition>,
    "GetPosition", sol::c_call<decltype(&Rigidbody::GetPosition), &Rigidbody::GetPosition>,
    "SetRotation", sol::c_call<decltype(&Rigidbody::SetRotation), &Rigidbody::SetRotation>,
    "GetRotation", sol::c_call<decltype(&Rigidbody::GetRotation), &Rigidbody::GetRotation>,
    "SetVelocity", sol::c_call<decltype(&Rigidbody::SetVelocity), &Rigidbody::SetVelocity>,
    "GetVelocity", sol::c_call<decltype(&Rigidbody::GetVelocity), &Rigidbody::GetVelocity>,
    "SetAngularVelocity", sol::c_call<decltype(&Rigidbody::SetAngularVelocity), &Rigidbody::SetAngularVelocity>,
    "GetAngularVelocity", sol::c_call<decltype(&Rigidbody::GetAngularVelocity), &Rigidbody::GetAngularVelocity>,
    "SetGravityScale", sol::c_call<decltype(&Rigidbody::SetGravityScale), &Rigidbody::SetGravityScale>,
    "GetGravityScale", sol::c_call<decltype(&Rigidbody::GetGravityScale), &Rigidbody::GetGravityScale>,
    "SetUpDirection", sol::c_call<decltype(&Rigidbody::SetUpDirection), &Rigidbody::SetUpDirection>,
    "GetUpDirection", sol::c_call<decltype(&Rigidbody::GetUpDirection), &Rigidbody::GetUpDirection>,
    "SetRightDirection", sol::c_call<decltype(&Rigidbody::SetRightDirection), &Rigidbody::SetRightDirection>,
    "GetRightDirection",sol::c_call<decltype(&Rigidbody::GetRightDirection), &Rigidbody::GetRightDirection>,
    "GetMass", sol::c_call<decltype(&Rigidbody::GetMass), &Rigidbody::GetMass>);
    
    
    L.new_usertype<ITween>("Tween",
    "Play", sol::c_call<decltype(&ITween::cppTweenPlay), &ITween::cppTweenPlay>,
    "Pause", sol::c_call<decltype(&ITween::cppTweenPause), &ITween::cppTweenPause>,
    "Rewind", sol::c_call<decltype(&ITween::cppTweenRewind), &ITween::cppTweenRewind>,
    "Kill", sol::c_call<decltype(&ITween::cppTweenKill), &ITween::cppTweenKill>,
    "OnKill", sol::c_call<decltype(&ITween::cppTweenOnKill), &ITween::cppTweenOnKill>,
    "SetOvershoot", sol::c_call<decltype(&ITween::cppTweenSetOvershootOrAmplitude), &ITween::cppTweenSetOvershootOrAmplitude>,
    "SetAmplitude", sol::c_call<decltype(&ITween::cppTweenSetOvershootOrAmplitude), &ITween::cppTweenSetOvershootOrAmplitude>,
    "SetTimescale", sol::c_call<decltype(&ITween::cppTweenSetTimescale), &ITween::cppTweenSetTimescale>,
    "SetLoops", sol::c_call<decltype(&ITween::cppTweenSetLoops), &ITween::cppTweenSetLoops>,
    "SetEase", sol::c_call<decltype(&ITween::cppTweenSetEase), &ITween::cppTweenSetEase>,
    "SetUpdate", sol::c_call<decltype(&ITween::cppTweenSetUpdate), &ITween::cppTweenSetUpdate>,
    "SetAxisConstraint", sol::c_call<decltype(&ITween::cppTweenSetAxisConstraint), &ITween::cppTweenSetAxisConstraint>,
    "SetSnapping", sol::c_call<decltype(&ITween::cppTweenSetSnapping), &ITween::cppTweenSetSnapping>);
    
    
    L.new_usertype<b2Vec2>("Vector2",
    sol::call_constructor, sol::factories(
    []() { return b2Vec2(); },
    [](float x, float y) { return b2Vec2(x, y); }),
    "x", &b2Vec2::x,
    "y", &b2Vec2::y,
    "Normalize", sol::c_call<decltype(&b2Vec2::Normalize), &b2Vec2::Normalize>,
    "Length", sol::c_call<decltype(&b2Vec2::Length), &b2Vec2::Length>,
    sol::meta_function::addition,  [](const b2Vec2 &lhs, const b2Vec2 &rhs){ return lhs.operator_add(rhs); },
    sol::meta_function::subtraction,  [](const b2Vec2 &lhs, const b2Vec2 &rhs){ return lhs.operator_sub(rhs); },
    sol::meta_function::multiplication,  [](const b2Vec2 &lhs, const float a){ return lhs.operator_mul(a); },
    "Distance", sol::c_call<decltype(b2Distance), b2Distance>,
    "Dot", static_cast<float (*)(const b2Vec2&, const b2Vec2&)>(&b2Dot));
    
    
    L["Actor"] = L.create_table_with(
    "Find", sol::c_call<decltype(SceneManager::cppActorFind), SceneManager::cppActorFind>,
    "FindAll", sol::c_call<decltype(SceneManager::cppActorFindAll), SceneManager::cppActorFindAll>,
    "Instantiate", sol::c_call<decltype(SceneManager::cppActorInstantiate), SceneManager::cppActorInstantiate>,
    "Destroy", sol::c_call<decltype(SceneManager::cppActorDestroy), SceneManager::cppActorDestroy>);
    
    
    L["Application"] = L.create_table_with(
    "Quit", sol::c_call<decltype(cppApplicationQuit), cppApplicationQuit>,
    "OpenURL", sol::c_call<decltype(cppApplicationOpenURL), cppApplicationOpenURL>);
    
    
    L["Audio"] = L.create_table_with(
    "Play", sol::c_call<decltype(AudioManager::cppAudioPlay), AudioManager::cppAudioPlay>,
    "Halt", sol::c_call<decltype(AudioManager::cppAudioHalt), AudioManager::cppAudioHalt>,
    "SetVolume", sol::c_call<decltype(AudioManager::cppAudioSetVolume), AudioManager::cppAudioSetVolume>);


    L["AxisConstraint"] = L.create_table_with(
    "None", AxisConstraint::None,
    "X", AxisConstraint::X,
    "Y", AxisConstraint::Y);
    
    
    L["Camera"] = L.create_table_with(
    "SetPosition", sol::c_call<decltype(Engine::cppCameraSetPosition), Engine::cppCameraSetPosition>,
    "GetPositionX", sol::c_call<decltype(Engine::cppCameraGetPositionX), Engine::cppCameraGetPositionX>,
    "GetPositionY", sol::c_call<decltype(Engine::cppCameraGetPositionY), Engine::cppCameraGetPositionY>,
    "SetZoom", sol::c_call<decltype(Renderer::cppCameraSetZoom), Renderer::cppCameraSetZoom>,
    "GetZoom", sol::c_call<decltype(Renderer::cppCameraGetZoom), Renderer::cppCameraGetZoom>);
    
    
    L["Debug"] = L.create_table_with(
    "Log", sol::c_call<decltype(cppDebugLog), cppDebugLog>,
    "LogError", sol::c_call<decltype(cppDebugLogError), cppDebugLogError>);
    
    
    L["GOTween"] = L.create_table_with(
    "To", sol::overload(cppGOTweenTo<b2Vec2>, cppGOTweenTo<float>));
    
    
    L["EaseType"] = L.create_table_with(
    "Linear", EaseType::Linear,
    "InQuad", EaseType::InQuad,
    "OutQuad", EaseType::OutQuad,
    "InOutQuad", EaseType::InOutQuad,
    "InCubic", EaseType::InCubic,
    "OutCubic", EaseType::OutCubic,
    "InOutCubic", EaseType::InOutCubic,
    "InQuart", EaseType::InQuart,
    "OutQuart", EaseType::OutQuart,
    "InOutQuart", EaseType::InOutQuart,
    "InQuint", EaseType::InQuint,
    "OutQuint", EaseType::OutQuint,
    "InOutQuint", EaseType::InOutQuint,
    "InSine", EaseType::InSine,
    "OutSine", EaseType::OutSine,
    "InOutSine", EaseType::InOutSine,
    "InExpo", EaseType::InExpo,
    "OutExpo", EaseType::OutExpo,
    "InOutExpo", EaseType::InOutExpo,
    "InCirc", EaseType::InCirc,
    "OutCirc", EaseType::OutCirc,
    "InOutCirc", EaseType::InOutCirc,
    "InElastic", EaseType::InElastic,
    "OutElastic", EaseType::OutElastic,
    "InOutElastic", EaseType::InOutElastic,
    "InBack", EaseType::InBack,
    "OutBack", EaseType::OutBack,
    "InOutBack", EaseType::InOutBack,
    "InBounce", EaseType::InBounce,
    "OutBounce", EaseType::OutBounce,
    "InOutBounce", EaseType::InOutBounce);
    
    
    L["Event"] = L.create_table_with(
    "Publish", sol::c_call<decltype(EventBus::cppEventPublish), EventBus::cppEventPublish>,
    "Subscribe", sol::c_call<decltype(EventBus::cppEventSubscribe), EventBus::cppEventSubscribe>,
    "Unsubscribe", sol::c_call<decltype(EventBus::cppEventUnsubscribe), EventBus::cppEventUnsubscribe>);
    
    
    L["Image"] = L.create_table_with(
    "Draw", sol::c_call<decltype(ImageManager::cppImageDraw), ImageManager::cppImageDraw>,
    "DrawEx", sol::c_call<decltype(ImageManager::cppImageDrawEx), ImageManager::cppImageDrawEx>,
    "DrawUI", sol::c_call<decltype(ImageManager::cppImageDrawUI), ImageManager::cppImageDrawUI>,
    "DrawUIEx", sol::c_call<decltype(ImageManager::cppImageDrawUIEx), ImageManager::cppImageDrawUIEx>,
    "DrawPixel", sol::c_call<decltype(Renderer::cppImageDrawPixel), Renderer::cppImageDrawPixel>);
    
    
    L["Input"] = L.create_table_with(
    "GetKey", sol::c_call<decltype(Input::cppInputGetKey), Input::cppInputGetKey>,
    "GetKeyDown", sol::c_call<decltype(Input::cppInputGetKeyDown), Input::cppInputGetKeyDown>,
    "GetKeyUp", sol::c_call<decltype(Input::cppInputGetKeyUp), Input::cppInputGetKeyUp>,
    "GetMouseButton", sol::c_call<decltype(Input::cppInputGetMouseButton), Input::cppInputGetMouseButton>,
    "GetMouseButtonDown", sol::c_call<decltype(Input::cppInputGetMouseButtonDown), Input::cppInputGetMouseButtonDown>,
    "GetMouseButtonUp", sol::c_call<decltype(Input::cppInputGetMouseButtonUp), Input::cppInputGetMouseButtonUp>,
    "GetMousePosition", sol::c_call<decltype(Input::cppInputGetMousePosition), Input::cppInputGetMousePosition>,
    "GetMouseScrollDelta", sol::c_call<decltype(Input::cppInputGetMouseScrollDelta), Input::cppInputGetMouseScrollDelta>);


    L["LoopType"] = L.create_table_with(
    "Restart", LoopType::Restart,
    "Yoyo", LoopType::Yoyo,
    "Incremental", LoopType::Incremental);
    
    
    L["Physics"] = L.create_table_with(
    "Raycast", sol::c_call<decltype(cppPhysicsRaycast), cppPhysicsRaycast>,
    "RaycastAll", sol::c_call<decltype(cppPhysicsRaycastAll), cppPhysicsRaycastAll>);
    
    
    L["Scene"] = L.create_table_with(
    "Load", sol::c_call<decltype(Engine::cppSceneLoad), Engine::cppSceneLoad>,
    "GetCurrent", sol::c_call<decltype(SceneManager::cppSceneGetCurrent), SceneManager::cppSceneGetCurrent>,
    "DontDestroy", sol::c_call<decltype(SceneManager::cppSceneDontDestroy), SceneManager::cppSceneDontDestroy>);
    
    
    L["Text"] = L.create_table_with("Draw", sol::c_call<decltype(TextManager::cppTextDraw), TextManager::cppTextDraw>);
    
    
    L["Time"] = L.create_table_with(
    "DeltaTime", sol::c_call<decltype(Engine::GetDeltaTime), Engine::GetDeltaTime>,
    "FixedDeltaTime", sol::c_call<decltype(Engine::GetFixedDeltaTime), Engine::GetFixedDeltaTime>);


    L["UpdateType"] = L.create_table_with(
    "Normal", UpdateType::Normal,
    "Late", UpdateType::Late,
    "Fixed", UpdateType::Fixed);
}


std::shared_ptr<Component> ComponentManager::MakeComponent(const std::shared_ptr<Component> &other_component) { return other_component->Clone(); }


std::shared_ptr<Component> ComponentManager::MakeComponent(const std::string &component_type)
{
    if (__native_component_factory.count(component_type) > 0)
        return __native_component_factory[component_type]();
    
    return std::make_shared<LuaComponent>(component_type);
}


std::shared_ptr<Component> ComponentManager::MakeComponent(const rapidjson::Value &component_json)
{
    std::string component_type = component_json.HasMember("type") && component_json["type"].IsString() ? component_json["type"].GetString() : "";
    
    std::shared_ptr<Component> component_ptr = MakeComponent(component_type);
    
    component_ptr->UpdateComponentWithJSON(component_json);
    
    return component_ptr;
}


void ComponentManager::EstablishInheritance(sol::table &instance_table, const std::string &component_name)
{
    sol::table component_table;
    if(L[component_name].valid())
    {
        component_table = L.globals()[component_name];
        EstablishInheritance(instance_table, component_table);
    }
    else
    {
        std::string component_path = COMPONENTS_PATH + component_name + ".lua";
        if (!fs::exists(component_path))
            ErrorExit("error: failed to locate component " + component_name);
        else
        {
            if (!L.script_file(component_path).valid())
                ErrorExit("problem with lua file " + component_name);
            
            component_table = L.globals()[component_name];
            EstablishInheritance(instance_table, component_table);
        }
    }
}

template <typename T>
ITween* ComponentManager::cppGOTweenTo(sol::table tween_table, sol::protected_function getter, sol::protected_function setter, T end, float duration)
{
    std::function<T()> getfunc = [tween_table, getter]() { return getter(tween_table); };
    std::function<void(T)> setfunc = [tween_table, setter](T x) { setter(tween_table, x); };
    T start = getfunc();
    
    return TweenManager::GOTo(getfunc, setfunc, start, end, duration);
}

//void ComponentManager::UpdateTweens()
//{
//    std::for_each(active_float_tweens.begin(), active_float_tweens.end(), [](TestTween<float> &active_float_tween) { active_float_tween.UpdateTween(); });
//    
//    active_float_tweens.erase(std::remove_if(active_float_tweens.begin(), active_float_tweens.end(), [](TestTween<float> &active_float_tween) { return active_float_tween.loops == 0; }), active_float_tweens.end());
//    
//    std::for_each(active_vector2_tweens.begin(), active_vector2_tweens.end(), [](TestTween<b2Vec2> &active_vector2_tween) { active_vector2_tween.UpdateTween(); });
//    
//    active_vector2_tweens.erase(std::remove_if(active_vector2_tweens.begin(), active_vector2_tweens.end(), [](TestTween<b2Vec2> &active_vector2_tween) { return active_vector2_tween.loops == 0; }), active_vector2_tweens.end());
//}
