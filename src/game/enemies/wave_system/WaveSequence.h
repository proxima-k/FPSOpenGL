#pragma once

#include <iostream>
#include <vector>

#include "Wave.h"

class WaveSequence {
public:
	WaveSequence(std::vector<Wave*> wave) 
		: enemiesToSpawn(wave) {}

	void update(float dt);

	std::vector<Wave*> enemiesToSpawn;
};

