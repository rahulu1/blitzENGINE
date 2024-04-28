//
//  AnimationManager.cpp
//  blitzENGINE
//
//  Created by Rahul Unniyampath on 4/27/24.
//

#include "AnimationManager.hpp"

#include <filesystem>

namespace fs = std::filesystem;


std::shared_ptr<Animation> AnimationManager::GetAnimation(const std::string &animation_name)
{
    if (animation_cache.count(animation_name) > 0)
        return animation_cache[animation_name];
    else
        return LoadAnimation(animation_name);
}

std::shared_ptr<Animation> AnimationManager::LoadAnimation(const std::string &animation_name)
{
    std::string animation_path = ANIMATION_PATH + animation_name + ".reel";
    
    if (!fs::exists(animation_path))
       ErrorExit("error: animation " + animation_name + " is missing");
    
    // Read scene JSON
    rapidjson::Document animation_doc;
    ReadJsonFile(animation_path, animation_doc);
    
    
    animation_cache.emplace(std::make_pair(animation_name, Animation::CreateAnimationFromJSON(animation_doc)));
    
    return animation_cache[animation_name];
}
