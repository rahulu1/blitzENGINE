//
//  Animation.cpp
//  blitzENGINE
//
//  Created by Rahul Unniyampath on 4/27/24.
//

#include "Animation.hpp"


Animation::Animation() {}


std::shared_ptr<Animation> Animation::CreateAnimationFromJSON(const rapidjson::Value &animation_json)
{
    std::shared_ptr<Animation> new_animation = std::make_shared<Animation>();
    
    new_animation->UpdateAnimationWithJSON(animation_json);
    
    return new_animation;
}


void Animation::UpdateAnimationWithJSON(const rapidjson::Value &animation_json)
{
    fps = animation_json.HasMember("fps") && animation_json["fps"].IsNumber() ? static_cast<uint16_t>(animation_json["fps"].GetFloat()) : fps;
    
    
    if (animation_json.HasMember("keyframes") && animation_json["keyframes"].IsArray())
    {
        for (const auto &keyframe_json : animation_json["keyframes"].GetArray())
        {
            uint16_t keyframe_number = keyframe_json["frame_index"].GetInt();
            
            if ((keyframe_number + 1) > total_frames)
            {
                total_frames = keyframe_number + 1;
                frame_names.resize(static_cast<size_t>(total_frames));
//                frames.resize(static_cast<size_t>(total_frames));
            }
            
            std::string image_name = keyframe_json["image_name"].GetString();
            
            frame_names[keyframe_number] = image_name; // TODO: Replace this + next with commented code below after integrating SpriteRenderer
            
            ImageManager::GetImage(image_name);
            
//            std::shared_ptr<Image> keyframe_image = ImageManager::GetImage(image_name);
//            
//            frames[keyframe_number] = keyframe_image;
        }
    }
    
    total_frames = animation_json.HasMember("total_frames") && animation_json["total_frames"].IsInt() ? static_cast<uint32_t>(animation_json["total_frames"].GetInt()) : total_frames;
    
    frame_names.resize(static_cast<size_t>(total_frames));
    
    std::string last_keyframe_name = *frame_names.begin();
    
    for (int frame_number = 0; frame_number < total_frames; frame_number++)
    {
        if (!frame_names[frame_number].empty())
            last_keyframe_name = frame_names[frame_number]; // if there is a keyframe here, it's the new latest keyframe
        else
            frame_names[frame_number] = last_keyframe_name; // if no frame here, set image to last keyframe
            
    }
    
//    std::shared_ptr<Image> last_keyframe = *frames.begin();
//    
//    for (int frame_number = 0; frame_number <= total_frames; frame_number++)
//    {
//        if (frames[frame_number])
//            last_keyframe = frames[frame_number]; // if there is a keyframe here, it's the new latest keyframe
//        else
//            frames[frame_number] = last_keyframe; // if no frame here, set image to last keyframe
//            
//    }
}


std::string Animation::GetImageNameAtSpecifiedFrame(uint32_t frame_index)
{
    return frame_names[frame_index];
}


uint32_t Animation::GetTotalFrames() const
{
    return total_frames;
}

float Animation::GetFPS() const
{
    return fps;
}
