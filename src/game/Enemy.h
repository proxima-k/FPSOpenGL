#pragma once

#include "Game.h"
#include "Entity.h"
#include "Physicsbody.h"
#include "xp/XP.h"


class Enemy : public Entity 
{
public:
	Enemy(glm::vec3 position, MeshRenderer meshRenderer);
	
	void spawnXP(glm::vec3 position, int xpamount) 
	{
		game->spawn_entity<XP>(position, meshRenderer);
	}

	float health;
	float maxHealth;
};