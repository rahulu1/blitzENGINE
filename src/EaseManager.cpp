//
//  EaseManager.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#include "EaseManager.hpp"

#include "glm.hpp"
#include "box2d.h"

float EaseManager::EvaluateEase(EaseType ease_type, float elapsed_time, float duration, float overshootOrAmplitude)
{
    float s;
    float period = 0.3f;
    
    switch (ease_type)
    {
        case EaseType::Linear:
            return elapsed_time / duration;

        case EaseType::InQuad:
            elapsed_time /= duration;
            return elapsed_time * elapsed_time;

        case EaseType::OutQuad:
            elapsed_time /= duration;
            return -elapsed_time * (elapsed_time - 2);

        case EaseType::InOutQuad:
            elapsed_time /= duration * 0.5f;
            if (elapsed_time < 1) return 0.5f * elapsed_time * elapsed_time;
            --elapsed_time;
            return -0.5f * (elapsed_time * (elapsed_time - 2) - 1);

        case EaseType::InCubic:
            elapsed_time /= duration;
            return elapsed_time * elapsed_time * elapsed_time;

        case EaseType::OutCubic:
            elapsed_time = elapsed_time / duration - 1;
            return elapsed_time * elapsed_time * elapsed_time + 1;

        case EaseType::InOutCubic:
            elapsed_time /= duration * 0.5f;
            if (elapsed_time < 1) return 0.5f * elapsed_time * elapsed_time * elapsed_time;
            elapsed_time -= 2;
            return 0.5f * (elapsed_time * elapsed_time * elapsed_time + 2);

        case EaseType::InQuart:
            elapsed_time /= duration;
            return elapsed_time * elapsed_time * elapsed_time * elapsed_time;

        case EaseType::OutQuart:
            elapsed_time = elapsed_time / duration - 1;
            return -(elapsed_time * elapsed_time * elapsed_time * elapsed_time - 1);

        case EaseType::InOutQuart:
            elapsed_time /= duration * 0.5f;
            if (elapsed_time < 1) return 0.5f * elapsed_time * elapsed_time * elapsed_time * elapsed_time;
            elapsed_time -= 2;
            return -0.5f * (elapsed_time * elapsed_time * elapsed_time * elapsed_time - 2);

        case EaseType::InQuint:
            elapsed_time /= duration;
            return elapsed_time * elapsed_time * elapsed_time * elapsed_time * elapsed_time;

        case EaseType::OutQuint:
            elapsed_time = elapsed_time / duration - 1;
            return elapsed_time * elapsed_time * elapsed_time * elapsed_time * elapsed_time + 1;

        case EaseType::InOutQuint:
            elapsed_time /= duration * 0.5f;
            if (elapsed_time < 1) return 0.5f * elapsed_time * elapsed_time * elapsed_time * elapsed_time * elapsed_time;
            elapsed_time -= 2;
            return 0.5f * (elapsed_time * elapsed_time * elapsed_time * elapsed_time * elapsed_time + 2);

        case EaseType::InSine:
            return -glm::cos(elapsed_time / duration * (b2_pi * 0.5f)) + 1;

        case EaseType::OutSine:
            return glm::sin(elapsed_time / duration * (b2_pi * 0.5f));

        case EaseType::InOutSine:
            return -0.5f * (glm::cos(b2_pi * elapsed_time / duration) - 1);

        case EaseType::InExpo:
            return (elapsed_time == 0) ? 0 : glm::pow(2, 10 * (elapsed_time / duration - 1));

        case EaseType::OutExpo:
            return (elapsed_time == duration) ? 1 : (-glm::pow(2, -10 * elapsed_time / duration) + 1);

        case EaseType::InOutExpo:
            if (elapsed_time == 0) return 0;
            if (elapsed_time == duration) return 1;
            if ((elapsed_time /= duration * 0.5f) < 1) return 0.5f * glm::pow(2, 10 * (elapsed_time - 1));
            return 0.5f * (-glm::pow(2, -10 * --elapsed_time) + 2);

        case EaseType::InCirc:
            elapsed_time /= duration;
            return -(glm::sqrt(1 - elapsed_time * elapsed_time) - 1);

        case EaseType::OutCirc:
            elapsed_time = elapsed_time / duration - 1;
            return glm::sqrt(1 - elapsed_time * elapsed_time);

        case EaseType::InOutCirc:
            elapsed_time /= duration * 0.5f;
            if (elapsed_time < 1)
                return -0.5f * (glm::sqrt(1 - elapsed_time * elapsed_time) - 1);
            elapsed_time -= 2;
            return 0.5f * (glm::sqrt(1 - elapsed_time * elapsed_time) + 1);

        case EaseType::InElastic:
            if (elapsed_time == 0) return 0;
            elapsed_time /= duration;
            if (elapsed_time == 1) return 1;
            if (overshootOrAmplitude < 1) {
                overshootOrAmplitude = 1;
                s = period / 4;
            } else {
                s = period * glm::asin(1 / overshootOrAmplitude) / (2 * b2_pi);
            }
            elapsed_time -= 1;
            return -(overshootOrAmplitude * glm::pow(2, 10 * elapsed_time) * glm::sin((elapsed_time * duration - s) * (2 * b2_pi) / period));

        case EaseType::OutElastic:
            if (elapsed_time == 0) return 0;
            elapsed_time /= duration;
            if (elapsed_time == 1) return 1;
            if (overshootOrAmplitude < 1) {
                overshootOrAmplitude = 1;
                s = period / 4;
            } else {
                s = period * glm::asin(1 / overshootOrAmplitude) / (2 * b2_pi);
            }
            return (overshootOrAmplitude * glm::pow(2, -10 * elapsed_time) * glm::sin((elapsed_time * duration - s) * (2 * b2_pi) / period) + 1);

        case EaseType::InOutElastic:
            if (elapsed_time == 0) return 0;
            elapsed_time /= (duration * 0.5f);
            if (elapsed_time == 2) return 1;
            if (overshootOrAmplitude < 1) {
                overshootOrAmplitude = 1;
                s = period / 4;
            } else {
                s = period * glm::asin(1 / overshootOrAmplitude) / (2 * b2_pi);
            }
            if (elapsed_time < 1) {
                elapsed_time -= 1;
                return -0.5f * (overshootOrAmplitude * glm::pow(2, 10 * elapsed_time) * glm::sin((elapsed_time * duration - s) * (2 * b2_pi) / period));
            }
            elapsed_time -= 1;
            return overshootOrAmplitude * glm::pow(2, -10 * elapsed_time) * glm::sin((elapsed_time * duration - s) * (2 * b2_pi) / period) * 0.5f + 1;

        case EaseType::InBack:
            elapsed_time /= duration;
            return elapsed_time * elapsed_time * ((1.70158f + 1) * elapsed_time - 1.70158f);

        case EaseType::OutBack:
            elapsed_time = elapsed_time / duration - 1;
            return elapsed_time * elapsed_time * ((1.70158f + 1) * elapsed_time + 1.70158f) + 1;

        case EaseType::InOutBack:
            elapsed_time /= (duration * 0.5f);
            if (elapsed_time < 1)
            {
                overshootOrAmplitude *= 1.525f;
                return 0.5f * (elapsed_time * elapsed_time * ((overshootOrAmplitude + 1) * elapsed_time - overshootOrAmplitude));
            }
            elapsed_time -= 2;
            overshootOrAmplitude *= 1.525f;
            return 0.5f * (elapsed_time * elapsed_time * ((overshootOrAmplitude + 1) * elapsed_time + overshootOrAmplitude) + 2);

        case EaseType::InBounce:
            return 1 - EaseManager::EvaluateEase(EaseType::OutBounce, duration - elapsed_time, duration, overshootOrAmplitude);

        case EaseType::OutBounce:
            elapsed_time /= duration;
            if (elapsed_time < (1 / 2.75f))
                return 7.5625f * elapsed_time * elapsed_time;
            if (elapsed_time < (2 / 2.75f))
            {
                elapsed_time -= (1.5f / 2.75f);
                return 7.5625f * elapsed_time * elapsed_time + 0.75f;
            }
            if (elapsed_time < (2.5 / 2.75))
            {
                elapsed_time -= (2.25f / 2.75f);
                return 7.5625f * elapsed_time * elapsed_time + 0.9375f;
            }
            elapsed_time -= (2.625f / 2.75f);
            return 7.5625f * elapsed_time * elapsed_time + 0.984375f;

        case EaseType::InOutBounce:
            if (elapsed_time < duration * 0.5f) return EaseManager::EvaluateEase(EaseType::InBounce, elapsed_time * 2, duration, overshootOrAmplitude) * 0.5f;
            return EaseManager::EvaluateEase(EaseType::OutBounce, elapsed_time * 2 - duration, duration, overshootOrAmplitude) * 0.5f + 0.5f;
        
        default:
            return EvaluateEase(EaseType::OutQuad, elapsed_time, duration, overshootOrAmplitude);
    }
}
