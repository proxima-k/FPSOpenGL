#include "AudioManager.h"

#define DR_MP3_IMPLEMENTATION
#include<openAL/alc.h>
#include<openAL/dr_mp3.h>

#define OpenAL_ErrorCheck(message)\
{\
    ALenum error = alGetError();\
    if (error != AL_NO_ERROR)\
    {\
        std::cout << "OpenAL Error: " << error << std::endl;\
        std::cout << message << std::endl;\
    }\
}

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(#FUNCTION_CALL)

bool AudioManager::isPlaying(std::string filename) {
    if (soundSources.find(filename) != soundSources.end()) {
        ALint state;
        alGetSourcei(soundSources[filename], AL_SOURCE_STATE, &state);
        return (state == AL_PLAYING);
    }
    return false;
}



void AudioManager::playSound(std::string filename, glm::vec3 location, float volume) {
    // check if the sound is already playing
    if (isPlaying(filename)) {
        std::cout << "Sound " << filename << " is already playing." << std::endl;
        return;
    }

    const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    ALCdevice* device = alcOpenDevice(defaultDeviceString);
    if (!device) {
        std::cerr << "Failed to get the default device for OpenAL" << std::endl;
        return;
    }
    std::cout << "OpenAL Device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;

    ALCcontext* context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context)) {
        std::cerr << "Failed to make the OpenAL context the current context" << std::endl;
        return;
    }

    alec(alListener3f(AL_POSITION, 0.f, 0.f, 0.f));
    alec(alListener3f(AL_VELOCITY, 0.f, 0.f, 0.f));
    ALfloat forwardAndUpVectors[] = { 1.f, 0.f, 0.f, 0.f, 1.f, 0.f };
    alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));

    drmp3_config config;
    drmp3_uint64 totalPCMFrameCount;
    std::string path = "res/audio/" + filename;
    drmp3_int16* pSampleData = drmp3_open_file_and_read_pcm_frames_s16(path.c_str(), &config, &totalPCMFrameCount, nullptr);
    if (!pSampleData) {
        std::cerr << "Failed to load audio file" << std::endl;
        return;
    }

    std::vector<uint16_t> pcmData(static_cast<size_t>(totalPCMFrameCount * config.channels));
    std::memcpy(pcmData.data(), pSampleData, pcmData.size() * sizeof(drmp3_int16));
    drmp3_free(pSampleData, nullptr);

    ALuint buffer;
    alec(alGenBuffers(1, &buffer));
    alec(alBufferData(buffer, config.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, pcmData.data(),
        static_cast<ALsizei>(pcmData.size() * sizeof(uint16_t)), config.sampleRate));

    ALuint source;
    alec(alGenSources(1, &source));
    alec(alSourcef(source, AL_PITCH, 1.f));
    alec(alSourcef(source, AL_GAIN, 1.f));
    alec(alSource3f(source, AL_POSITION, location.x, location.y, location.z));
    alec(alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f));
    alec(alSourcei(source, AL_LOOPING, AL_FALSE));
    alec(alSourcei(source, AL_BUFFER, buffer));

    soundSources[filename] = source;  // track the source by filename

    alec(alSourcePlay(source));
}