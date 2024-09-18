#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <openAL/al.h>
#include <openAL/alc.h>
#include "AudioClip.h"
#include "glm/glm.hpp"

#define OpenAL_ErrorCheck(message) \
{ \
    ALenum error = alGetError(); \
    if (error != AL_NO_ERROR) \
    { \
        std::cerr << "OpenAL Error: " << error << std::endl; \
        std::cerr << message << std::endl; \
    } \
}

#define alec(FUNCTION_CALL) \
FUNCTION_CALL; \
OpenAL_ErrorCheck(#FUNCTION_CALL)

class AudioManager {
public:
    void init();
    void cleanup();
    void playSound(AudioClip* audioClip, glm::vec3 location, float volume);
    AudioClip* getAudioClip(const std::string& fileName);

private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;
    std::vector<AudioClip*> audioClips;
    std::vector<ALuint> sources;
    std::vector<ALuint> buffers;
};

#endif
