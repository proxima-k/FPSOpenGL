#include "WaveController.h"

#include "../game/Game.h"

#include "../CosineEnemy.h"
#include "../SineEnemy.h"
#include "../CubeEnemy.h"

WaveController::WaveController() {
	populate_queue();
}

void WaveController::populate_queue() {
	std::vector<Wave*> waveOne = {
	new Wave(EnemyType::Basic, 2.0f)
	};
	waveQueue.push(create_wave_sequence(waveOne));

	std::vector<Wave*> waveTwo = {
		new Wave(EnemyType::Basic, 1.0f),
		new Wave(EnemyType::Cosine, 4.0f)
	};
	waveQueue.push(create_wave_sequence(waveTwo));

	std::vector<Wave*> waveThree = {
		new Wave(EnemyType::Basic, 2.0f),
		new Wave(EnemyType::Cosine, 2.0f),
		new Wave(EnemyType::Sinus, 2.0f)
	};
	waveQueue.push(create_wave_sequence(waveThree));
}

std::vector<Wave*> WaveController::create_wave(std::vector<EnemyType> enemyTypes, std::vector<float> spawnInterval) {
	std::vector<Wave*> newWaveList;

	for (size_t i = 0; i < enemyTypes.size(); i++) {
		newWaveList.push_back(new Wave(enemyTypes[i], spawnInterval[i]));
	}

	return newWaveList;
}

WaveSequence WaveController::create_wave_sequence(std::vector<Wave*> waves) {
	WaveSequence newWaveSequence(waves);
	return newWaveSequence;
}

void WaveController::update(float dt) {
	if (waveQueue.empty()) return;

	waveQueue.front().update(dt);

	auto sequence = waveQueue.front();
	for (size_t index = 0; index < sequence.enemiesToSpawn.size(); index++) {
		auto* wave = sequence.enemiesToSpawn[index];
		if (spawnedOnce) return;
		if (wave->currentTime <= 0) {
			spawn_enemy_by_type(wave->enemyToSpawn);
			spawnedOnce = true;
			wave->currentTime = wave->spawnInterval;
		}
	}
}

void WaveController::next_wave() {
	if(waveQueue.size() > 1)
	waveQueue.pop();
}

void WaveController::reset_waves() {
	while (!waveQueue.empty()) {
		waveQueue.pop();
	}

	populate_queue();
}

void WaveController::spawn_enemy_by_type(EnemyType enemyType) {
	glm::vec3 spawnPos = game->player->transform.position + game->player->transform.getRandomPointInRadius(10, 25);
	
	switch (enemyType) {
	case EnemyType::Basic:
		game->spawn_entity<CubeEnemy>(spawnPos);
		break;
	case EnemyType::Cosine:
		game->spawn_entity<CosineEnemy>(spawnPos);
		break;
	case EnemyType::Sinus:
		game->spawn_entity<SineEnemy>(spawnPos);
		break;
	default:
		std::cerr << "Unknown enemy type!" << std::endl;
		break;
	}
}
