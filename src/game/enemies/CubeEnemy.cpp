#include <iostream>

#include "CubeEnemy.h"
#include "../Game.h"

CubeEnemy::CubeEnemy(glm::vec3 position, MeshRenderer meshRenderer)
	: Enemy(position, meshRenderer), physicsBody(), trailRenderer()
{
	Shader* trailShader = new Shader("res/shaders/Basic.shader");
	TrailMesh* trailMesh = new TrailMesh({});

	trailMesh->maxTrailPoints = 25;

	trailRenderer.setMesh(trailMesh);
	trailRenderer.setShader(trailShader);
	trailRenderer.setTargetCamera(game->camera);
	trailRenderer.setColor(meshRenderer.getColor());

	collision_channel = Collision_Channel::Enemy;

	xpAmount = 50.f;
}


void CubeEnemy::update(float deltaTime)
{
	glm::vec3 camPos = Camera::mainCamera->transform.position;
	glm::vec3 dir = glm::normalize(camPos - transform.position);
	 
	glm::vec3 speed = glm::vec3(20);

	physicsBody.acceleration = glm::vec3(0.0f);
	physicsBody.acceleration += dir * speed;

	transform.position += physicsBody.velocity * deltaTime;
	transform.rotation = glm::quatLookAt(glm::normalize(dir), glm::vec3(0, 1, 0));

	if (glm::distance(lastPosition, transform.position) > 0.05f) 
	{
		TrailMesh* trailMesh = trailRenderer.getMesh();

		glm::vec3 newPos = transform.position;

		trailMesh->addVertex(glm::vec3(newPos), .1f);
		
		lastPosition = transform.position;
	}

	physicsBody.update();

	trailRenderer.draw(transform.position, transform.rotation, glm::vec3(1.0f));
}