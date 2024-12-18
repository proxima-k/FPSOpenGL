#include <iostream>

#include "Game.h"

#include "Player.h"
#include "enemies/CubeEnemy.h"
#include "CreditsObject.h"

#include "AABB.h"
#include "OBB.h"

#include "../graphics/MeshRenderer.h"
#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"

#include "Grid.h"
#include "YAxisLine.h"

Game::Game()
{
	for (int i = 0; i < MAX_ENTITYS; ++i)
	{
		entitys[i] = nullptr;  // initialize all pointers to nullptr
	}

	textureManager = new TextureManager;
	textureManager->init();
}

Game::~Game()
{
	for (int i = 0; i < MAX_ENTITYS; ++i)
	{
		delete entitys[i];
		entitys[i] = nullptr;
	}

	delete textureManager;
	delete waveController;
	delete bossFightController;
	delete gameStateManager;
}

void Game::update()
{
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;


	// update game states here
	if (gameStateManager != nullptr)
		gameStateManager->update(deltaTime);


	// updates all the entitys
	for (int i = 0; i < MAX_ENTITYS; i++) 
	{
		if (entitys[i] == nullptr) continue;

		if (entitys[i]->destroyed) {
			delete entitys[i];
			entitys[i] = nullptr;
		}
		else {
			if (!isInGame()) continue;
			entitys[i]->update(deltaTime);
		}
	}

	// handles healing line updates and destruction
	for (int i = 0; i < MAX_HEAL_LINES; i++) {
		
		if (healingLines[i] == nullptr) continue;

		if (healingLines[i]->destroyed) {
			//std::cout << "deleting healing line" << std::endl;
			delete healingLines[i];
			healingLines[i] = nullptr;
		}
		else {
			if (!isInGame()) continue;
			healingLines[i]->update(deltaTime);
		}
	}

	for (int i = 0; i < MAX_PARTICLE_CTRLS; i++)
	{
		auto* pCtrl = particleCtrl[i];

		if (pCtrl == nullptr) continue;

		if (pCtrl->isEmpty()) {
			delete pCtrl;
			particleCtrl[i] = nullptr;
			return;
		}

		if (!isInGame()) continue;
		pCtrl->update(deltaTime);
	}

	bossFightController->update(deltaTime);
	floorGrid->update();
	/*if (currentGameState != GameStates::SelectCards && currentGameState != GameStates::Menu) {
		timeLeftUntilBoss -= deltaTime;
		waveController->update(deltaTime);
	}*/
}

// calls the draw function on all the entities
void Game::render()
{
	for (int i = 0; i < MAX_ENTITYS; i++)
	{
		if (entitys[i] == nullptr || entitys[i]->destroyed)
			continue;

		entitys[i]->draw();
	}

	for (int i = 0; i < MAX_PARTICLE_CTRLS; i++)
	{
		auto* pCtrl = particleCtrl[i];

		if (pCtrl != nullptr)
			pCtrl->render();
	}

	if (player->canDrawBody)
		player->draw();
}

void Game::renderHealingLines() {
	for (int i = 0; i < MAX_HEAL_LINES; i++) {
		if (healingLines[i] == nullptr) continue;
		healingLines[i]->draw();
	}
}

void Game::renderGrid() {
	bossFightController->drawBossCage();
	if (!bossFightController->bossFightIsActive()) {
		if (floorGrid != nullptr)
			floorGrid->draw();
		if (yAxisLine != nullptr)
			yAxisLine->draw();
	}
}

// compares the bounding boxes of all the entities to see if they are overlapping
Entity* Game::get_coliding_entity(Entity* other, Collision_Channel channel) 
{
	for (int i = 0; i < MAX_ENTITYS; i++) 
	{
		if (entitys[i] == nullptr || entitys[i] == other || entitys[i]->collision_channel != channel || entitys[i]->destroyed)
			continue;

		AABB a = AABB::from_position_size(other->transform);
		AABB b = AABB::from_position_size(entitys[i]->transform);

		if (aabb_overlap(a, b)) {
			return entitys[i];
		}
	}

	return nullptr;
}

Entity* Game::get_colliding_entity_OBB(Entity* self, Collision_Channel channel)
{
	for (int i = 0; i < MAX_ENTITYS; i++) {
		if (entitys[i] == nullptr || entitys[i] == self || entitys[i]->collision_channel != channel || entitys[i]->destroyed)
			continue;

		OBB::OBB obb1(&(self->transform)	  , self->transform.scale);
		OBB::OBB obb2(&(entitys[i]->transform), entitys[i]->transform.scale);

		if (OBB::isOverlapping(obb1, obb2)) {
			return entitys[i];
		}
	}

	return nullptr;
}

void Game::enterSelectCardState()
{
	if (crtPlayerXP >= maxPlayerXP) {
		clearEnemies();
		crtPlayerXP = 0;
		level_up_player();
		//currentGameState = GameStates::SelectCards;
		gameStateManager->changeState(GameStateManager::State::SelectCards);
	}
}

void Game::gameOver()
{
	//currentGameState = Menu;

	/*for (int i = 0; i < MAX_ENTITYS; i++)
	{
		if (entitys[i] != nullptr)
		{
			entitys[i]->destroy();
		}
	}*/

	gameStateManager->changeState(GameStateManager::State::MainMenu);
}

void Game::reset()
{
	crtPlayerXP = 0;
	maxPlayerXP = 100;
	playerLevel = 1;

	timeLeftUntilBoss = (minutesUntilBossSpawns * 60) + 1;
	//timeLeftUntilBoss = 30.f;

	playerDamageMultiplier = 1.0f;
	playerSpeedMultiplier = 1.0f;
	playerDashMultiplier = 1.0f;

	bGameOver = false;
	bWin = false;

	player->reset();
	waveController->reset_waves();

	//GameStates currentGameState = GameStates::Playing;

	for (int i = 0; i < MAX_ENTITYS; ++i)
	{
		delete entitys[i];
		entitys[i] = nullptr;
	}
}

void Game::clearEnemies() {
	for (int i = 0; i < MAX_ENTITYS; i++) {
		if (entitys[i] != nullptr) {
			if (entitys[i]->collision_channel == Collision_Channel::Enemy) {
				/*entitys[i]->destroyed = true;*/
				delete entitys[i];
				entitys[i] = nullptr;
			}
		}
	}
}

void Game::startCredits() {

	if (bWin) return;

	glm::vec3 spawnPos = glm::vec3(player->transform.position + (player->transform.getForward() * glm::vec3(20))) + glm::vec3(0, 20, 0);
	auto credits = spawn_entity<CreditObject>(spawnPos);

	glm::vec3 direction = player->transform.position - credits->transform.position;
	direction.y = 0.0f;
	direction = glm::normalize(direction);

	credits->transform.rotation = glm::quatLookAt(-direction, glm::vec3(0, 1, 0));

	bWin = true;
}
