#pragma once

#include "Game.h"
#include "Entity.h"
#include "Physicsbody.h"
#include "xp/XP.h"


class Enemy : public Entity 
{
public:
	Enemy(glm::vec3 position);
	
	float health;
	float maxHealth;
	float xpAmount;

	// deal damage to enemy
	virtual void take_damage(int damage)
	{
		health -= damage;
		if (health <= 0)
		{
			die(xpAmount);
		}
	}

	// so all enemies simply can call die() to be removed from the game
	virtual void die(float xpSpawnAmount)
	{
		game->addPlayerXP(xpSpawnAmount);
		game->spawn_particle_source(transform.position, 40.0f, 0.3f, 2.0f);
		destroy();
	}
};