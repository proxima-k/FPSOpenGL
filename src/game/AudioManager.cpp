#include "AudioManager.h"
#include "openAL/dr_mp3.h"
#include <filesystem>

const std::filesystem::path filePath = "res/audio/";

void AudioManager::init() {

    drmp3_config config;
    drmp3_uint64 totalPCMFrameCount;

    std::vector<std::string> fileNames;
    std::filesystem::path folderPath = std::filesystem::current_path() / filePath;

    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            std::string textureName = entry.path().stem().string();
            fileNames.push_back(textureName);
        }
    }

    std::string path = "res/audio/";

    for (const auto& fileName : fileNames) {

        std::string fullPath = path + fileName + ".mp3";
        drmp3_int16* audioData = drmp3_open_file_and_read_pcm_frames_s16(fullPath.c_str(), &config, &totalPCMFrameCount, nullptr);
        if (!audioData) {
            std::cerr << "Failed to load audio file: " << fullPath << std::endl;
            continue;
        }

        auto audioClip = new AudioClip();
        audioClip->config = config;
        audioClip->totalPCMFrameCount = totalPCMFrameCount;
        audioClip->audioData = audioData;
        audioClip->fileName = fileName;

        audioClips.push_back(audioClip);
    }

    const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    device = alcOpenDevice(defaultDeviceString);
    if (!device) {
        std::cerr << "Failed to open default device" << std::endl;
        return;
    }

    context = alcCreateContext(device, nullptr);
    if (!alcMakeContextCurrent(context)) {
        std::cerr << "Failed to make OpenAL context current" << std::endl;
        alcCloseDevice(device);
        device = nullptr;
        return;
    }

    std::cout << "Audio Loaded" << std::endl;
}

//get audio clip by file name
AudioClip* AudioManager::getAudioClip(const std::string& fileName) {
    for (auto& audioClip : audioClips) {
        if (audioClip->fileName == fileName) {
            std::cout << "Success" << std::endl;
            return audioClip;
        }
    }
    return nullptr;
}

void AudioManager::playSound(AudioClip* audioClip, glm::vec3 location, float volume) {
    if (!audioClip) {
        std::cerr << "Invalid AudioClip" << std::endl;
        return;
    }

    ALuint buffer;
    alec(alGenBuffers(1, &buffer));
    buffers.push_back(buffer);

    drmp3_config config = audioClip->config;
    drmp3_uint64 totalPCMFrameCount = audioClip->totalPCMFrameCount;
    drmp3_int16* audioData = audioClip->audioData;

    std::vector<uint16_t> pcmData(static_cast<size_t>(totalPCMFrameCount * config.channels));
    std::memcpy(pcmData.data(), audioData, pcmData.size() * sizeof(drmp3_int16));

    alec(alBufferData(buffer, config.channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16,
        pcmData.data(), static_cast<ALsizei>(pcmData.size() * sizeof(uint16_t)), config.sampleRate));

    ALuint source;
    alec(alGenSources(1, &source));
    sources.push_back(source);

    alec(alSourcef(source, AL_PITCH, 1.f));
    alec(alSourcef(source, AL_GAIN, volume));
    alec(alSource3f(source, AL_POSITION, location.x, location.y, location.z));
    alec(alSource3f(source, AL_VELOCITY, 0.f, 0.f, 0.f));
    alec(alSourcei(source, AL_LOOPING, AL_FALSE));
    alec(alSourcei(source, AL_BUFFER, buffer));

    alec(alSourcePlay(source));

    activeSourceBuffers.push_back({ source, buffer });

    std::cout << "Playing" << std::endl;
}

void AudioManager::update() {
    auto it = activeSourceBuffers.begin();
    while (it != activeSourceBuffers.end()) {
        ALint sourceState;
        alec(alGetSourcei(it->source, AL_SOURCE_STATE, &sourceState));

        if (sourceState == AL_STOPPED) {
            alec(alSourceStop(it->source));
            alec(alDeleteSources(1, &it->source));

            if (alIsBuffer(it->buffer)) {
                alec(alDeleteBuffers(1, &it->buffer));
            }

            it = activeSourceBuffers.erase(it);
        }
        else {
            it++;
        }
    }
}
