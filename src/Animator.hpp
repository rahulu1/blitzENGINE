//
//  Animator.hpp
//  blitzENGINE
//
//  Created by Rahul Unniyampath on 4/27/24.
//

#ifndef Animator_hpp
#define Animator_hpp

#include "AnimationManager.hpp"
#include "NativeComponent.hpp"

class Animator : public NativeComponent {
public:
    
    
    Animator();
    
    
    Animator(const Animator &other);
    
    
    void UpdateComponentWithJSON(const rapidjson::Value &component_json) override;
    
    
    std::shared_ptr<Component> Clone() const override;
    
    
    std::shared_ptr<Component> GetSharedPointer() override;
    
    
    Animator* cppAnimatorPlay();
    
    
    Animator* cppAnimatorPause();
    
    
    Animator* cppAnimatorRewind();
    
    
    void cppAnimatorKill();
    
    
    void cppAnimatorIsPlaying() const;
    
    
    Animator* SetAnimation(const std::string &animation_name);
    
    
    Animator* cppAnimatorSetLoops(int32_t loops, LoopType loop_type);
    
    
    std::string cppAnimatorGetAnimation() const;
    
    
    Animator* cppAnimatorSetFrameIndex(float frame_number);
    
    
    float GetCurrentFrameIndex() const;
    
    
    std::string GetCurrentFrameName() const; // TODO: Swap back to GetCurrentFrame after integrating SpriteRenderer
    
    
//    std::shared_ptr<Image> GetCurrentFrame() const;
    
    
    Animator* cppAnimatorSetTimescale(float timescale);
    
    
    float cppAnimatorGetTimescale() const;
    
    
    Animator* cppAnimatorOnKill(sol::protected_function kill_func, sol::optional<sol::table> kill_table);
    
    
    void OnStart() override;
    
    
    void OnFixedUpdate() override;
    
    
    void OnUpdate() override;
    
    
    void OnLateUpdate() override;
    
    
    void OnCollisionEnter(const CollisionData &collision) override;
    
    
    void OnCollisionExit(const CollisionData &collision) override;
    
    
    void OnTriggerEnter(const CollisionData &collision) override;
    
    
    void OnTriggerExit(const CollisionData &collision) override;
    
    
    void OnDestroy() override;
    
    
    void TweenCurrentFrameIndex(float new_frame_index);
    
private:
    
    
    std::string current_animation_name;
    
    
    std::shared_ptr<Animation> current_animation;
    
    
    std::shared_ptr<Tween<float>> frame_tween;
    
    
    uint32_t current_frame_index;
    
}; /* Animator */


inline std::shared_ptr<Component> Animator::Clone() const { return std::make_shared<Animator>(*this); }


inline std::shared_ptr<Component> Animator::GetSharedPointer() { return shared_from_this(); }


inline Animator* Animator::cppAnimatorPlay()
{
    frame_tween->Play();
    return this;
}


inline Animator* Animator::cppAnimatorPause()
{
    frame_tween->Pause();
    return this;
}


inline Animator* Animator::cppAnimatorRewind()
{
    frame_tween->Rewind();
    return this;
}


inline void Animator::cppAnimatorKill() { frame_tween->Kill(); }


inline void Animator::cppAnimatorIsPlaying() const
{
    frame_tween->IsPlaying();
}


inline std::string Animator::cppAnimatorGetAnimation() const
{
    return current_animation_name;
}


inline Animator* Animator::cppAnimatorSetLoops(int32_t loops, LoopType loop_type)
{
    frame_tween->SetLoops(loops, loop_type);
    return this;
}


inline Animator* Animator::cppAnimatorSetFrameIndex(float frame_number)
{
    float total_frames = static_cast<float>(current_animation->GetTotalFrames());
    
    float new_progress = frame_number / total_frames;
    
    frame_tween->SetProgress(new_progress);
    
    current_frame_index = std::roundf(new_progress * total_frames);
    
    return this;
}


inline float Animator::GetCurrentFrameIndex() const
{
    return current_frame_index;
}

// TODO: Replace with appropriate function after integrating SpriteRenderer
inline std::string Animator::GetCurrentFrameName() const
{
    std::string current_frame_name = !current_animation_name.empty() ? current_animation->GetImageNameAtSpecifiedFrame(current_frame_index) : "";
    return current_frame_name;
}


inline Animator* Animator::cppAnimatorSetTimescale(float timescale)
{
    frame_tween->SetTimescale(timescale);
    
    return this;
}


inline float Animator::cppAnimatorGetTimescale() const
{
    return frame_tween->GetTimescale();
}


inline Animator* Animator::cppAnimatorOnKill(sol::protected_function kill_func, sol::optional<sol::table> kill_table)
{
    frame_tween->OnKill(kill_func, kill_table);
    
    return this;
}


inline void Animator::OnStart() {}


inline void Animator::OnFixedUpdate() {}


inline void Animator::OnUpdate() {}


inline void Animator::OnLateUpdate() {}


inline void Animator::OnCollisionEnter(const CollisionData &collision) {}


inline void Animator::OnCollisionExit(const CollisionData &collision) {}


inline void Animator::OnTriggerEnter(const CollisionData &collision) {}


inline void Animator::OnTriggerExit(const CollisionData &collision) {}


inline void Animator::OnDestroy()
{
    frame_tween->Kill();
}


inline void Animator::TweenCurrentFrameIndex(float new_frame_index)
{
    current_frame_index = static_cast<uint32>(new_frame_index);
}


#endif /* Animator_hpp */
