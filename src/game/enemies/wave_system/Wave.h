#pragma once

#include <vector>

enum EnemyType {
	Basic,
	Cosine,
	Sinus,
	Pillar
};

class Wave {
public:
	Wave(EnemyType enemyType, float spawnInt)
		: enemyToSpawn(enemyType), spawnInterval(spawnInt), currentTime(spawnInt) { }

	EnemyType enemyToSpawn;
	float spawnInterval;
    float currentTime;
};