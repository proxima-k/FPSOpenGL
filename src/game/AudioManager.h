#pragma once

#define DR_MP3_IMPLEMENTATION
#include<openAL/al.h>
#include<openAL/alc.h>
#include<openAL/dr_mp3.h>
#include<vector>
#include<string>
#include<iostream>
#include<glm/glm.hpp>

class AudioManager {
public :
	void playSound(std::string filename, glm::vec3 location);
};