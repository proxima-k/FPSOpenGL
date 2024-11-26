#include "MainMenuState.h"

#include <glm/glm.hpp>

#include "../Player.h"
#include "../Camera.h"

MainMenuState::MainMenuState(GameStateManager* gameStateManager)
	: gameStateManager(gameStateManager)
{
}

void MainMenuState::onEnterState()
{
	player = gameStateManager->game->player;
	camera = gameStateManager->game->camera;

	timeElapsed = 0.f;
	t = 0.f;

}

void MainMenuState::onUpdateState(float deltaTime)
{
	// get reference 
	glm::vec3 offsetFromPlayer = glm::vec3(glm::cos(timeElapsed), 0, glm::sin(timeElapsed)) * 5.f + glm::vec3(0.f,1.8f,0.f);

	
	cameraSlotTf.position = player->transform.position + offsetFromPlayer;
	cameraSlotTf.lookAt(player->transform.position);
	//camera->transform.position = player->transform.position + offsetFromPlayer;
	//camera->transform.lookAt(player->transform.position);
	
	// lerp camera position and rotation towards cameraSlot
	camera->transform.position = glm::mix(camera->transform.position, cameraSlotTf.position, t);
	camera->transform.rotation = glm::slerp(camera->transform.rotation, cameraSlotTf.rotation, t);

	if (t < 1) {
		t += deltaTime / 10.f;
	}

	timeElapsed += deltaTime / 5.f;
}

void MainMenuState::onExitState()
{
	//camera->transform.position = player->transform.position;
	//camera->lookAt(player->transform.position);
}
