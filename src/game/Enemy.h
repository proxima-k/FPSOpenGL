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

	// so all enemies simply can call die() to be removed from the game
	void die(int xpAmount) 
	{
		game->spawnXP(transform.position, xpAmount);
		destroy();
	}
};