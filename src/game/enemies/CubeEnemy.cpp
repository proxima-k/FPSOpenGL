#include "CubeEnemy.h"

#include <iostream>

CubeEnemy::CubeEnemy(glm::vec3 position, Mesh* mesh, Shader* shader, Camera* camera)
	: Enemy(position, mesh, shader, camera), physicsBody(), camera(camera) {	}


void CubeEnemy::update(float deltaTime)
{
	glm::vec3 camPos = camera->transform.position;
	glm::vec3 dir = glm::normalize(camPos - transform.position);

	physicsBody.acceleration = glm::vec3(0.0f);

	physicsBody.acceleration += dir * glm::vec3(10,10,10);
	transform.position += physicsBody.velocity * deltaTime;

	physicsBody.update();
}