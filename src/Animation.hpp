//
//  Animation.hpp
//  blitzENGINE
//
//  Created by Rahul Unniyampath on 4/27/24.
//

#ifndef Animation_hpp
#define Animation_hpp

#include "ImageManager.hpp"
#include "TweenManager.hpp"

#include <cstdint>
#include <memory>
#include <vector>

class Animation : std::enable_shared_from_this<Animation> {
public:
    
    
    Animation();
    
    
    static std::shared_ptr<Animation> CreateAnimationFromJSON(const rapidjson::Value &animation);
    
    // TODO: Replace with appropriate function after integrating SpriteRenderer
    std::string GetImageNameAtSpecifiedFrame(uint32_t frame_index);
    
    
    uint32_t GetTotalFrames() const;
    
    
    float GetFPS() const;
    
private:
    
    
    void UpdateAnimationWithJSON(const rapidjson::Value &animation_json);
    
    
    std::vector<std::string> frame_names; // TODO: Replace with 'frames' map after integrating SpriteRenderer
//    std::vector<std::shared_ptr<Image>> frames;
    
    
    uint32_t total_frames = 0;
    
    
    float fps = 30;
};

#endif /* Animation_hpp */
