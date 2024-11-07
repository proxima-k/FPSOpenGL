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
	Spawner(float spawnRate, Player* player) :
		spawnRate(spawnRate), timer(spawnRate), player(player),
		// passing in a type for the spawn function
		spawnFunction([=](glm::vec3 pos) -> void* { return static_cast<void*>(game->spawn_entity<EntityType>(pos)); }) 
	{}

	void update(float deltaTime) {
		timer -= deltaTime;

		if (timer < 0.f) {
			// calculate random position around the player to spawn the enemy
			glm::vec3 spawnPos = player->transform.position + player->transform.getRandomPointInRadius(10, 25);

			spawnFunction(spawnPos);
			timer = spawnRate;
		}
	}	

private:
	std::function<void* (glm::vec3)> spawnFunction;
	float spawnRate;
	float timer;
	Player* player;
	// potential: how many to spawn per timer reaches zero
	// potential: spawn distance bounds
};
