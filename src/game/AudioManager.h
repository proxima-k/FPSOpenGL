#pragma once

#include<vector>
#include<string>
#include<iostream>
#include<glm/glm.hpp>
#include<unordered_map>
#include<openAL/al.h>

class AudioManager {
public:
    void playSound(std::string filename, glm::vec3 location, float volume);
    bool isPlaying(std::string filename);

private:
    std::unordered_map<std::string, ALuint> soundSources;  // Track sources by filename
};