#pragma once

#include "Game.h"
#include "Entity.h"
#include "Physicsbody.h"
#include "xp/XP.h"


class Enemy : public Entity 
{
public:
	Enemy(glm::vec3 position, MeshRenderer meshRenderer);
	
	float health;
	float maxHealth;
	float xpAmount = 1.f;

	// so all enemies simply can call die() to be removed from the game
	void die(float xpSpawnAmount) 
	{
		game->addPlayerXP(xpAmount);
		game->spawnXP(transform.position, xpSpawnAmount);
		destroy();
	}
};