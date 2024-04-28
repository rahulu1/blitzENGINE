//
//  TweenManager.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#include "TweenManager.hpp"

#include "Engine.h"


template <typename T>
std::shared_ptr<Tween<T>> TweenManager::GOTo(std::function<T ()> getter, std::function<void (T)> setter, T start, T end, float duration)
{
    std::shared_ptr<Tween<T>> new_tween = Tween<T>::CreateTween(getter, setter, start, end, duration);

    updating_tweens.insert(new_tween);
    
    return new_tween;
}


void TweenManager::Update()
{
    float elapsed_time = Engine::GetDeltaTime();

    for (auto it = updating_tweens.begin(); it != updating_tweens.end();)
    {
        if ((*it)->TweenCompleted())
        {
            it = updating_tweens.erase(it);
            continue;
        }

        if ((*it)->IsPlaying())
            (*it)->EvaluateAndApply(elapsed_time);
        
        ++it;
    }
}


void TweenManager::LateUpdate()
{
    float elapsed_time = Engine::GetDeltaTime();

    for (auto it = late_updating_tweens.begin(); it != late_updating_tweens.end();)
    {
        if ((*it)->TweenCompleted())
        {
            it = late_updating_tweens.erase(it);
            continue;
        }

        if ((*it)->IsPlaying())
            (*it)->EvaluateAndApply(elapsed_time);
        
        ++it;
    }
}


void TweenManager::FixedUpdate()
{
    float elapsed_time = Engine::GetFixedDeltaTime();

    for (auto it = fixed_updating_tweens.begin(); it != fixed_updating_tweens.end();)
    {
        if ((*it)->TweenCompleted())
        {
            it = fixed_updating_tweens.erase(it);
            continue;
        }

        if ((*it)->IsPlaying())
            (*it)->EvaluateAndApply(elapsed_time);
        
        ++it;
    }
}


template std::shared_ptr<Tween<float>> TweenManager::GOTo<float>(std::function<float()> getter, std::function<void(float)> setter, float start, float end, float duration);

template std::shared_ptr<Tween<b2Vec2>> TweenManager::GOTo<b2Vec2>(std::function<b2Vec2()> getter, std::function<void(b2Vec2)> setter, b2Vec2 start, b2Vec2 end, float duration);
