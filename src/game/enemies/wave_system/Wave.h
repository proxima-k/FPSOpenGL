#pragma once

#include <vector>

enum EnemyType {
	Basic,
	Cosine,
	Sinus,
	Pillar,
	Caterpillar,
	Bomber,
	Propeller,
};

class Wave {
public:
	Wave(EnemyType enemyType, float spawnInt)
		: enemyToSpawn(enemyType), spawnInterval(spawnInt), currentTime(spawnInt) { }

	EnemyType enemyToSpawn;
	float spawnInterval;
    float currentTime;
};