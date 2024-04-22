//
//  AudioManager.cpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/11/24.
//

#include "AudioManager.hpp"

#include <filesystem>

namespace fs = std::filesystem;


Mix_Chunk* AudioManager::LoadSound(const std::string &audioName)
{
    if (audioCache.find(audioName) == audioCache.end()) {
        std::string wavPath = AUDIOS_PATH + audioName + ".wav";
        std::string oggPath = AUDIOS_PATH + audioName + ".ogg";

        // Check for WAV file
        if (fs::exists(wavPath))
        {
            audioCache[audioName] = internalLoadSound(wavPath);
            return audioCache[audioName];
        }
        // Check for OGG file
        else if (fs::exists(oggPath))
        {
            audioCache[audioName] = internalLoadSound(oggPath);
            return audioCache[audioName];
        }
        // File not found
        else
            return nullptr;
    }
    return audioCache[audioName];
}
