#pragma once
#include "glm/glm.hpp"

class Physicsbody 
{
public:
	void update();

	void add_force(glm::vec3 force)
	{
		acceleration += force;
	}

	glm::vec3 velocity;
	glm::vec3 acceleration;

	bool bGravity = false;

	float dampening = 5.f;
	float friction = 1.f;

	float deltaTime = 0.f;
	float lastFrameTime = 0.f;
};