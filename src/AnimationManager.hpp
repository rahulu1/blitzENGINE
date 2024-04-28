//
//  AnimationManager.hpp
//  blitzENGINE
//
//  Created by Rahul Unniyampath on 4/27/24.
//

#ifndef AnimationManager_hpp
#define AnimationManager_hpp

#define ANIMATION_PATH "resources/animations/"

#include "Animation.hpp"

#include <map>
#include <memory>


class AnimationManager {
    
public:

    
    static std::shared_ptr<Animation> GetAnimation(const std::string &animation_name);
    
private:
    
    
    static std::shared_ptr<Animation> LoadAnimation(const std::string &animation_name);
    
    
    static inline std::map<std::string, std::shared_ptr<Animation>> animation_cache;
};

#endif /* AnimationManager_hpp */
