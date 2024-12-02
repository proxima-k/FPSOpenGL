#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <vector>
#include <unordered_map>
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

struct SourceBuffer {
    ALuint source;
    ALuint buffer;
};

class AudioManager {
public:
    ~AudioManager();
    void init();
    void update();
    void playSound(AudioClip* audioClip, glm::vec3 location, float volume);
    AudioClip* getAudioClip(const std::string& fileName);

private:
    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    std::vector<AudioClip*> audioClips;
    std::vector<SourceBuffer> activeSourceBuffers;

    std::unordered_map<std::string, ALuint> bufferCache;

    ALuint getBufferForClip(AudioClip* audioClip);

    void cleanupAudioClips();
    void cleanupActiveSources();
};

extern AudioManager* audioManager;

#endif // AUDIOMANAGER_H
