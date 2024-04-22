//
//  CollisionManager.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 3/30/24.
//

#ifndef CollisionManager_hpp
#define CollisionManager_hpp

#include "Actor.hpp"
#include "box2d.h"
#include "SceneManager.hpp"

#include <stdio.h>

struct CollisionData {
    
    
    Actor* other;
    
    
    b2Vec2 point = b2Vec2(-999.0f, -999.0f);
    
    
    b2Vec2 relative_velocity;
    
    
    b2Vec2 normal = b2Vec2(-999.0f, -999.0f);
    
};


class CollisionManager : public b2ContactListener   {
    
public:
    
    
    void BeginContact(b2Contact *contact) override;
    
    
    void EndContact(b2Contact *contact) override;
    
    
    static void ProcessContactCallbacks();
    
private:
    
    
    static inline std::vector<std::function<void()>> callbacks_to_process;
};

#endif /* CollisionManager_hpp */
