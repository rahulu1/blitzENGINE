//
//  Animator.cpp
//  blitzENGINE
//
//  Created by Rahul Unniyampath on 4/27/24.
//

#include "Animator.hpp"

Animator::Animator()
{
    type = "Animator";
    
    component_ref = sol::make_object(ComponentManager::GetLuaState()->lua_state(), this);
}


Animator::Animator(const Animator &other) :
NativeComponent(other),
current_animation_name(other.current_animation_name),
current_frame_index(0)
{
    SetAnimation(current_animation_name);
    
    component_ref = sol::make_object(ComponentManager::GetLuaState()->lua_state(), this);
}


void Animator::UpdateComponentWithJSON(const rapidjson::Value &component_json)
{
    if (component_json.HasMember("animation_name") && component_json["animation_name"].IsString())
    {
        current_animation_name = component_json["animation_name"].GetString();
        
        SetAnimation(current_animation_name);
        
        if (component_json.HasMember("auto_play") && component_json["auto_play"].IsBool())
        {
            if (!component_json["auto_play"].GetBool())
                frame_tween->Pause();
        }
        
        if (component_json.HasMember("timescale") && component_json["timescale"].IsNumber())
            frame_tween->SetTimescale(component_json["timescale"].GetFloat());
    }
}


Animator* Animator::SetAnimation(const std::string &animation_name)
{
    if (current_animation && frame_tween->IsPlaying())
        frame_tween->Stop();
    
    current_animation_name = animation_name;
    current_animation = AnimationManager::GetAnimation(current_animation_name);
    
    float total_frames = static_cast<float>(current_animation->GetTotalFrames() - 1);
    
    uint32_t fps = current_animation->GetFPS();
    
    float animation_duration = total_frames / static_cast<float>(fps);
    
    std::function<float()> frame_tween_getter = [this]() { return this->GetCurrentFrameIndex(); };
    
    std::function<void(float)> frame_tween_setter = [this](float new_frame_number) { TweenCurrentFrameIndex(new_frame_number); };
    
    frame_tween = TweenManager::GOTo(frame_tween_getter, frame_tween_setter, 0.0f, total_frames, animation_duration);
    
    frame_tween->SetEase(EaseType::Linear)->SetLoops(-1, LoopType::Restart)->SetSnapping(true)->SetUpdate(UpdateType::Late);
    
    return this;
}
