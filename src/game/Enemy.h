#pragma once

#include "Entity.h"
#include "Physicsbody.h"

class Enemy : public Entity 
{
public:
	Enemy(glm::vec3 position, MeshRenderer meshRenderer);

	float health;
	float maxHealth;
};