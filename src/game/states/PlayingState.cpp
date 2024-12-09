#include "PlayingState.h"

#include <glm/glm.hpp>

#include "../Player.h"
#include "../BossFightController.h"
#include "../Game.h"
#include "GameStateManager.h"

PlayingState::PlayingState(GameStateManager* gameStateManager)
	: gameStateManager(gameStateManager) {}

void PlayingState::onEnterState()
{
	game = gameStateManager->game;
	player = game->player;
	camera = game->camera;

	game->bossFightController->cleanUp();
	if (gameStateManager->getPreviousState() == GameStateManager::State::MainMenu) {
		player->reset();
		
		t = 0.f;
		player->canInput = false;
		startPointTf.position = player->transform.position;
		startPointTf.rotation = player->transform.rotation;
	}
	else {
		player->canInput = true;
		t = 1.f;
	}
}

void PlayingState::onUpdateState(float deltaTime)
{
	player->update(deltaTime);
	game->timeLeftUntilBoss -= deltaTime;
	//game->waveController->update(deltaTime);

	// lerp camera movement back to original
	// if t isn't at 1 then player cannot input
	if (t < 1.f) {
		t += deltaTime * 2.f;
		if (t >= 1) t = 1;
		camera->transform.position = glm::mix(camera->transform.position, startPointTf.position, t);
		camera->transform.rotation = glm::slerp(camera->transform.rotation, startPointTf.rotation, t);
	}
	else {
		player->canInput = true;
	}

	if (game->timeLeftUntilBoss <= 0) {
		game->clearEnemies();
		gameStateManager->changeState(GameStateManager::State::BossFight);
	}
}

void PlayingState::onExitState()
{
}
