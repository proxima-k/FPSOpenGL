#pragma once

#include <string>

#include <openAL/alc.h>
#include <openAL/al.h>
#include <openAL/dr_mp3.h>

class AudioClip {
public:
    drmp3_config config;
    drmp3_uint64 totalPCMFrameCount;
    drmp3_int16* audioData;
    std::string fileName;
};