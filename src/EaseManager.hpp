//
//  EaseManager.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#ifndef EaseManager_hpp
#define EaseManager_hpp

#include <stdio.h>

enum class EaseType
{
    Linear,
    InQuad,
    OutQuad,
    InOutQuad,
    InCubic,
    OutCubic,
    InOutCubic,
    InQuart,
    OutQuart,
    InOutQuart,
    InQuint,
    OutQuint,
    InOutQuint,
    InSine,
    OutSine,
    InOutSine,
    InExpo,
    OutExpo,
    InOutExpo,
    InCirc,
    OutCirc,
    InOutCirc,
    InElastic,
    OutElastic,
    InOutElastic,
    InBack,
    OutBack,
    InOutBack,
    InBounce,
    OutBounce,
    InOutBounce
};


class EaseManager {
public:
    static float EvaluateEase(EaseType ease_type, float elapsed_time, float duration, float overshootOrAmplitude);
};

#endif /* EaseManager_hpp */
