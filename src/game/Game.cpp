#include <iostream>

#include "Game.h"

#include "Player.h"
#include "enemies/CubeEnemy.h"

#include "AABB.h"

#include "../graphics/MeshRenderer.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"

Game::Game()
{
	for (int i = 0; i < MAX_ENTITYS; ++i)
	{
		entitys[i] = nullptr;  // initialize all pointers to nullptr
	}

	textureManager = new TextureManager;
	textureManager->init();

	audioManager = new AudioManager;
	audioManager->init();
}

Game::~Game()
{
	for (int i = 0; i < MAX_ENTITYS; ++i)
	{
		delete entitys[i];
		entitys[i] = nullptr;
	}
	
	delete textureManager;
	delete audioManager;
}

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

	audioManager->update();
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
			return entitys[i];
		}
	}

	return nullptr;
}

void Game::enterSelectCardState()
{
	if (crtPlayerXP >= maxPlayerXP) {
		for (int i = 0; i < MAX_ENTITYS; i++) {
			if (entitys[i] != nullptr) {
				if (entitys[i]->collision_channel == Collision_Channel::Enemy) {
					entitys[i]->destroyed = true;
				}
			}
		}
		crtPlayerXP = 0;
		scaleMaxPlayerXP();

		currentGameState = GameStates::SelectCards;
	}
}

void Game::gameOver()
{
	currentGameState = Menu;

	for (int i = 0; i < MAX_ENTITYS; i++)
	{
		if (entitys[i] != nullptr)
		{
			entitys[i]->destroy();
		}
	}
}

void Game::reset()
{
	crtPlayerXP = 0;
	maxPlayerXP = 100;

	playerDamageMultiplier = 1.0f;
	playerSpeedMultiplier = 1.0f;
	playerDashMultiplier = 1.0f;

	bGameOver = false;

	player->reset();

	GameStates currentGameState = GameStates::Playing;

	for (int i = 0; i < MAX_ENTITYS; ++i)
	{
		delete entitys[i];
		entitys[i] = nullptr;
	}
}

// saves the mesh, shader and camera to be used when spawning new entities
void Game::setMeshRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera)
{
	this->cubeEnemyMesh = cardMesh;
	this->cubeEnemyShader = cardShader;
	this->camera = camera;
}