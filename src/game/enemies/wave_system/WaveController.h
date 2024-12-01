#pragma once

#include <iostream>
#include <queue>

#include "WaveSequence.h"

class WaveController {
public:
	WaveController();
	void populate_queue();

	void update(float dt);

	void next_wave();
	void reset_waves();

	void spawn_enemy_by_type(EnemyType enemyType);

	std::vector<Wave*> create_wave(std::vector<EnemyType> enemyType, std::vector<float> spawnInterval);
	WaveSequence create_wave_sequence(std::vector<Wave*> waves);

private:

	bool spawnedOnce = false;
	std::queue<WaveSequence> waveQueue;
};