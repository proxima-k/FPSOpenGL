#include <iostream>

#include "Game.h"
#include "Player.h"
#include "AABB.h"
#include "../graphics/MeshRenderer.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "enemies/CubeEnemy.h"

void Game::update() 
{
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

	// updates all the entitys
	for (int i = 0; i < MAX_ENTITYS; i++)
	{
		if (entitys[i] != nullptr)
		{
			if (entitys[i]->destroyed)
			{
				delete entitys[i];
				entitys[i] = nullptr;
			}
			else 
			{
				entitys[i]->update(deltaTime);
			}
		}
	}

	// spawn a new enemy when the timer reaches zero
	if (timer.isZero())
	{
		timer.setTimer(250);
		timer.setCallback([this]() { this->timer_callback(); });
	}

	timer.updateTimer(deltaTime);
}

// callback function for the timer
void Game::timer_callback()
{
	MeshRenderer newMeshRenderer(cubeEnemyMesh, cubeEnemyShader, camera);
	Entity* newEnemy = game->spawn_entity<CubeEnemy>(glm::vec3(1), newMeshRenderer);
	newEnemy->transform.position = camera->transform.position + newEnemy->transform.getRandomPointInRadius(10, 25);
}

// calls the draw function on all the entities
void Game::render()
{
	for (int i = 0; i < MAX_ENTITYS; i++)
	{
		if (entitys[i] != nullptr)
		{
			entitys[i]->draw();
		}
	}
}

// compares the bounding boxes of all the entities to see if they are overlapping
Entity* Game::get_coliding_entity(Entity* other, Collision_Channel channel) 
{
	for (int i = 0; i < MAX_ENTITYS; i++) 
	{
		if (entitys[i] == other || entitys[i] == nullptr || entitys[i]->collision_channel != channel)
			continue;

		AABB a = AABB::from_position_size(other->transform);
		AABB b = AABB::from_position_size(entitys[i]->transform);

		if (aabb_overlap(a, b)) {
			std::cout << "Overlapping" << std::endl;
			return entitys[i];
		}
	}

	return nullptr;
}

// saves the mesh, shader and camera to be used when spawning new entities
void Game::setMeshRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera)
{
	this->cubeEnemyMesh = cardMesh;
	this->cubeEnemyShader = cardShader;
	this->camera = camera;
}