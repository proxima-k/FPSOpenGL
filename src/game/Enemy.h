#pragma once

#include "Entity.h"
#include "Physicsbody.h"

class Enemy : public Entity 
{
public:
	Enemy(glm::vec3 position, Mesh* mesh, Shader* shader, Camera* camera);

	float health;
	float maxHealth;
};