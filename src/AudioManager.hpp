//
//  AudioManager.hpp
//  game_engine
//
//  Created by Rahul Unniyampath on 2/11/24.
//

#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include "SDL2_mixer/SDL_mixer.h"
#include "Utilities.hpp"

#include <string>
#include <unordered_map>


const std::string AUDIOS_PATH = "resources/audio/";

class AudioManager {
public:
    static void Init();
    
    static Mix_Chunk* LoadSound(const std::string &audio_name);
    
    static void cppAudioPlay(float channel, const std::string &audio_name, bool does_loop);
    static void cppAudioHalt(float audio_channel);
    static void cppAudioSetVolume(float audio_channel, float volume);
    
    static void SetMasterVolume(int volume);

private:
    static Mix_Chunk* internalLoadSound(const std::string &fullPath);
    
    static inline std::unordered_map<std::string, Mix_Chunk*> audioCache;
    static inline std::unordered_map<std::string, int> audioChannels;
};


inline void AudioManager::Init()
{
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0)
        ErrorExit("SDL_mixer could not initialize! SDL_mixer Error: " + std::string(Mix_GetError()));
    
    Mix_AllocateChannels(16); // Allocate channels for multiple simultaneous sounds
}


inline void AudioManager::cppAudioPlay(float channel, const std::string &audio_name,  bool does_loop)
{
    LoadSound(audio_name);
    
    int loops = (int) does_loop * -1;
    Mix_PlayChannel(channel, audioCache[audio_name], loops);
    audioChannels[audio_name] = channel;
}


inline void AudioManager::cppAudioHalt(float audio_channel){ Mix_HaltChannel(static_cast<int>(audio_channel)); }


inline void AudioManager::cppAudioSetVolume(float audio_channel, float volume) { Mix_Volume(static_cast<int>(audio_channel), static_cast<int>(volume)); }


inline void AudioManager::SetMasterVolume(int volume) { Mix_Volume(-1, volume); }


inline Mix_Chunk* AudioManager::internalLoadSound(const std::string &fullPath)
{
    Mix_Chunk* newSound = Mix_LoadWAV(fullPath.c_str());
    if (!newSound)
        return nullptr;
    
    return newSound;
}

#endif // AUDIOMANAGER_H
