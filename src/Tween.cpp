//
//  Tween.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#include "Tween.hpp"

#include "box2d.h"
#include "TweenManager.hpp"


template <typename T>
Tween<T>::Tween(std::function<T()> getter, std::function<void(T)> setter, T start, T end, float duration) :
    getter(getter),
    setter(setter),
    start_val(start),
    end_val(end)
{
    this->duration = duration;
    evaluated_duration = duration / timescale;
}


template <typename T>
std::shared_ptr<Tween<T>> Tween<T>::CreateTween(std::function<T()> getter, std::function<void(T)> setter, T start, T end, float duration)
{
    return std::make_shared<Tween<T>>(getter, setter, start, end, duration);
}


std::shared_ptr<ITween> ITween::GetSharedPointer()
{
    return shared_from_this();
}


template <>
void Tween<b2Vec2>::EvaluateAndApply(float dt)
{
    float tween_multiplier = Evaluate(dt);

    if (tween_multiplier == -1.0f)
    {
        setter(end_val);

        if (on_kill)
            on_kill();

        return;
    }

    b2Vec2 current_val = start_val.operator_add(end_val.operator_sub(start_val).operator_mul(tween_multiplier));

    current_val = snapping ? b2Vec2(std::round(current_val.x), std::round(current_val.y)) : current_val;

    switch (axis_constraint)
    {
    case AxisConstraint::X:
        current_val.y = start_val.y;
        break;

    case AxisConstraint::Y:
        current_val.x = start_val.x;
        break;

    default:    
        break;
    }

    setter(current_val);
}


template <typename T>
void Tween<T>::EvaluateAndApply(float dt)
{
    float tween_multiplier = Evaluate(dt);

    if (tween_multiplier == -1.0f)
    {
        setter(end_val);

        if (on_kill)
            on_kill();
        
        return;
    }

    T current_val = start_val + (end_val - start_val) * tween_multiplier;
    current_val = snapping ? std::round(current_val) : current_val;
    setter(current_val);
}

template <typename T>
float Tween<T>::Evaluate(float dt)
{
    elapsed_time += dt;
    
    if (elapsed_time >= evaluated_duration)
    {
        loops_completed += elapsed_time / evaluated_duration;
        elapsed_time = std::remainderf(elapsed_time, evaluated_duration);

        if (loops >= 0 && loops_completed >= loops)
            return -1.0f;
        else
        {
            elapsed_time = 0;
            
            T temp_val;

            switch (loop_type)
            {
            case LoopType::Yoyo:
                temp_val = end_val;
                end_val = start_val;
                start_val = temp_val;
                break;

            case LoopType::Incremental:
                temp_val = end_val;
                end_val = end_val + (end_val - start_val);
                start_val = temp_val;
                break;
            
            default:
                break;
            }
        }
    }

    return EaseManager::EvaluateEase(ease_type, elapsed_time, evaluated_duration, overshootOrAmplitude);
}


bool ITween::TweenCompleted()
{
    return (loops >= 0) && (loops_completed >= loops);
}


bool ITween::IsPlaying()
{
    return playing;
}


ITween* ITween::Play()
{
    playing = true;
    return this;
}


ITween* ITween::Pause()
{
    playing = false;
    return this;
}


ITween* ITween::Rewind()
{
    elapsed_time = 0.0f;
    return this;
}


ITween* ITween::Stop()
{
    loops = 0;
    on_kill = NULL;
    
    return this;
}


ITween* ITween::Kill()
{
    loops = 0;
    
    return this;
}


ITween* ITween::OnKill(sol::protected_function kill_func, sol::optional<sol::table> kill_table)
{
    if (kill_table.has_value())
        on_kill = [kill_func, kill_table]() { kill_func(kill_table.value()); };
    else
        on_kill = [kill_func]() { kill_func(); };

    return this;
}


ITween* ITween::SetOvershootOrAmplitude(float overshootOrAmplitude)
{
    this->overshootOrAmplitude = overshootOrAmplitude;
    return this;
}


ITween* ITween::SetTimescale(float timescale)
{
    this->timescale = timescale;
    evaluated_duration = duration / timescale;
    return this;
}


float ITween::GetTimescale() const
{
    return timescale;
}


ITween* ITween::SetLoops(int32_t loops, LoopType loop_type)
{
    this->loops = loops;
    this->loop_type = loop_type;
    return this;
}


ITween* ITween::SetEase(EaseType ease_type)
{
    this->ease_type = ease_type;
    return this;
}


ITween* ITween::SetUpdate(UpdateType update_type)
{
    if (this->update_type != update_type)
    {
        std::shared_ptr<ITween> insertion_tween = GetSharedPointer();
        
        switch (update_type) {
            case UpdateType::Late:
                TweenManager::late_updating_tweens.insert(insertion_tween);
                break;
                
            case UpdateType::Fixed:
                TweenManager::fixed_updating_tweens.insert(insertion_tween);
                break;
                
            default:
                TweenManager::updating_tweens.insert(insertion_tween);
                break;
        }
        
        switch (this->update_type) {
            case UpdateType::Late:
                TweenManager::late_updating_tweens.erase(insertion_tween);
                break;
                
            case UpdateType::Fixed:
                TweenManager::fixed_updating_tweens.erase(insertion_tween);
                break;
                
            default:
                TweenManager::updating_tweens.erase(insertion_tween);
                break;
        }
        
        this->update_type = update_type;
    }
    
    return this;
}


ITween* ITween::SetAxisConstraint(AxisConstraint axis_constraint)
{
    this->axis_constraint = axis_constraint;
    return this;
}


ITween* ITween::SetSnapping(bool snapping)
{
    this->snapping = snapping;
    return this;
}


ITween* ITween::SetProgress(float progress)
{
    elapsed_time = progress / timescale;
    return this;
}


float ITween::GetProgress() const
{
    float progress = elapsed_time / evaluated_duration;
    return progress;
}


template std::shared_ptr<Tween<float>> Tween<float>::CreateTween(std::function<float()> getter, std::function<void(float)> setter, float start, float end, float duration);

template std::shared_ptr<Tween<b2Vec2>> Tween<b2Vec2>::CreateTween(std::function<b2Vec2()> getter, std::function<void(b2Vec2)> setter, b2Vec2 start, b2Vec2 end, float duration);
