//
//  TweenManager.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#ifndef TweenManager_hpp
#define TweenManager_hpp

#include "Tween.hpp"

#include <memory>
#include <unordered_set>

class TweenManager
{
public:
    
    template <typename T>
    static std::shared_ptr<Tween<T>> GOTo(std::function<T()> getter, std::function<void(T)> setter, T start, T end, float duration);

    
    static void Update();


    static void LateUpdate();


    static void FixedUpdate();


    static inline std::unordered_set<std::shared_ptr<ITween>> updating_tweens;


    static inline std::unordered_set<std::shared_ptr<ITween>> late_updating_tweens;
    

    static inline std::unordered_set<std::shared_ptr<ITween>> fixed_updating_tweens;
};

#endif /* TweenManager_hpp */
