#pragma once

#include <functional>
#include <glm/vec3.hpp>

#include "../graphics/MeshRenderer.h"
#include "Game.h"
#include "Player.h"

template<typename EntityType>
class Spawner
{
public:
	// constructor
	// a lambda function is created for the spawnFunction
	Spawner(float spawnRate, MeshRenderer meshRenderer, Player* player) :
		spawnFunction(
			[=](glm::vec3 pos, MeshRenderer meshR) -> void* {
				return static_cast<void*>(game->spawn_entity<EntityType>(pos, meshR));
			}
		),
		spawnRate(spawnRate), timer(spawnRate), enemyMeshRenderer(meshRenderer), player(player) {}

	void update(float deltaTime) {
		timer -= deltaTime;

		if (timer < 0.f) {
			// calculate random position around the player to spawn the enemy
			glm::vec3 spawnPos = player->transform.position + player->transform.getRandomPointInRadius(10, 25);

			spawnFunction(spawnPos, enemyMeshRenderer);
			timer = spawnRate;
		}
	}	

private:
	std::function<void* (glm::vec3, MeshRenderer)> spawnFunction;
	float spawnRate;
	float timer;
	MeshRenderer enemyMeshRenderer;
	Player* player;
	// potential: how many to spawn per timer reaches zero
	// potential: spawn distance bounds
};
