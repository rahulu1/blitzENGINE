//
//  Tween.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 4/21/24.
//

#ifndef Tween_hpp
#define Tween_hpp

#include "lua.hpp"
#include "sol/sol.hpp"
#include "EaseManager.hpp"

#include <cstdint>
#include <functional>


enum class LoopType
{
    Restart,
    Yoyo,
    Incremental
};

enum class UpdateType
{
    Normal,
    Late,
    Fixed
};

enum class AxisConstraint
{
    None,
    X,
    Y
};

class ITween : public std::enable_shared_from_this<ITween> {
public:
    
    std::shared_ptr<ITween> GetSharedPointer();

    virtual void EvaluateAndApply(float dt) = 0;

    bool TweenCompleted();
    
    bool IsPlaying();
    
    ITween* Play();

    ITween* Pause();

    ITween* Rewind();
    
    ITween* Stop();

    ITween* Kill();

    ITween* OnKill(sol::protected_function kill_func, sol::optional<sol::table> kill_table);

    ITween* SetOvershootOrAmplitude(float overshootOrAmplitude);

    ITween* SetTimescale(float timescale);
    
    float GetTimescale() const;

    ITween* SetLoops(int32_t loops, LoopType loop_type);

    ITween* SetEase(EaseType ease_type);

    ITween* SetUpdate(UpdateType update_type);
    
    ITween* SetAxisConstraint(AxisConstraint axis_constraint);

    ITween* SetSnapping(bool snapping);
    
    ITween* SetProgress(float progress);
    
    float GetProgress() const;
    
protected:
    
    std::function<void()> on_kill;
    
    float duration = 0.0f;

    float evaluated_duration = 0.0f;
    
    float elapsed_time = 0.0f;
    
    float overshootOrAmplitude = 1.70158f;
    
    float timescale = 1.0f;
    
    int32_t loops = 1;

    int32_t loops_completed = 0;
    
    LoopType loop_type = LoopType::Restart;

    EaseType ease_type = EaseType::OutQuad;

    UpdateType update_type = UpdateType::Normal;

    AxisConstraint axis_constraint = AxisConstraint::None;
    
    bool snapping = false;

    bool playing = true;
};


template <typename T>
class Tween : public ITween {
public:
    
    Tween(std::function<T()> getter, std::function<void(T)> setter, T start, T end, float duration);

    static std::shared_ptr<Tween<T>> CreateTween(std::function<T()> getter, std::function<void(T)> setter, T start, T end, float duration);
    
    void EvaluateAndApply(float dt) override;

    float Evaluate(float dt);

private:
    
    std::function<T()> getter;
    
    std::function<void(T)> setter;
    
    T start_val;
    
    T end_val;
};


#endif /* Tween_hpp */
