#include <iostream>

#include "CubeEnemy.h"
#include "../Game.h"

CubeEnemy::CubeEnemy(glm::vec3 position, MeshRenderer meshRenderer)
	: Enemy(position, meshRenderer), physicsBody() 
{
	collision_channel = Collision_Channel::Enemy;
}

void CubeEnemy::update(float deltaTime)
{
	glm::vec3 camPos = Camera::mainCamera->transform.position;
	glm::vec3 dir = glm::normalize(camPos - transform.position);

	glm::vec3 speed = glm::vec3(10);

	physicsBody.acceleration = glm::vec3(0.0f);
	physicsBody.acceleration += dir * speed;

	transform.position += physicsBody.velocity * deltaTime;
	transform.rotation = glm::quatLookAt(glm::normalize(dir), glm::vec3(0, 1, 0));

	physicsBody.update();

	Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Projectile);
	if (hit_actor != nullptr)
	{
		game->playerScore += 10;
		destroy();
	}
}