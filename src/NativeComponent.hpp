//
//  NativeComponent.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/27/24.
//

#ifndef NativeComponent_hpp
#define NativeComponent_hpp

#include "Actor.hpp"

#include <stdio.h>

class NativeComponent : public Component {

public:
    
    
    NativeComponent();
    
    
    NativeComponent(const NativeComponent &other);
    
    
    inline void SetEnabled(bool enabled) override;
    
    
    inline bool IsEnabled() const override;
    
    
    Actor* GetActor() const;
    
protected:
    
    
    bool enabled;
    
};


inline NativeComponent::NativeComponent()                                                   { SetEnabled(true); }


inline NativeComponent::NativeComponent(const NativeComponent &other) : Component(other)    { SetEnabled(true); }


inline void NativeComponent::SetEnabled(bool enabled)                                       { this->enabled = enabled; }


inline bool NativeComponent::IsEnabled() const                                              { return enabled; }


inline Actor* NativeComponent::GetActor() const                                             { return actor.lock().get(); }

#endif /* NativeComponent_hpp */
