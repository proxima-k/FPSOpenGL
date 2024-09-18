#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <vector>
#include <string>
#include <iostream>
#include <openAL/al.h>
#include <openAL/alc.h>
#include "AudioClip.h"
#include "glm/glm.hpp"

// Macro for checking OpenAL errors
#define OpenAL_ErrorCheck(message) \
{ \
    ALenum error = alGetError(); \
    if (error != AL_NO_ERROR) \
    { \
        std::cerr << "OpenAL Error: " << error << std::endl; \
        std::cerr << message << std::endl; \
    } \
}

// Macro for executing OpenAL functions and checking for errors
#define alec(FUNCTION_CALL) \
FUNCTION_CALL; \
OpenAL_ErrorCheck(#FUNCTION_CALL)

struct SourceBuffer {
    ALuint source;
    ALuint buffer;
};

class AudioManager {
public:
    void init();
    void update();
    void playSound(AudioClip* audioClip, glm::vec3 location, float volume);

    AudioClip* getAudioClip(const std::string& fileName);

private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    std::vector<AudioClip*> audioClips;

    // vectors to store OpenAL source and buffer IDs
    std::vector<ALuint> sources;
    std::vector<ALuint> buffers;

    // vector to track active source-buffer pairs for cleanup
    std::vector<SourceBuffer> activeSourceBuffers;
};

extern AudioManager* audioManager;
#endif
