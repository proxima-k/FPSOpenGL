#include "BossFightController.h"

#include "enemies/boss/BossCage.h"
#include "enemies/boss/BossEnemy.h"
#include "Player.h"
#include "Game.h"
#include "Camera.h"

#include <ShaderManager.h>

void BossFightController::update(float deltaTime)
{
	if (cage != nullptr)
		cage->update(deltaTime);
}

void BossFightController::initializeBossFight()
{
	if (hasInitialized) return;
	hasInitialized = true;

	// get player location
	Player* player = game->player;
	glm::vec3 cameraLookDirection = Camera::mainCamera->transform.getForward();
	cameraLookDirection.y = 0;
	cameraLookDirection = glm::normalize(cameraLookDirection);

	// todo: swap camera look direction to player forward
	// need to fix player rotate in player movement
	glm::vec3 cageSpawnLocation = player->transform.position + (cameraLookDirection * 6.f);
	cageSpawnLocation.y = 0;
	glm::vec2 spawnLocationInt = floorGrid->getCellCoords(cageSpawnLocation);
	cageSpawnLocation.x = spawnLocationInt.x;
	cageSpawnLocation.z = spawnLocationInt.y;

	cage = new BossCage(cageSpawnLocation, 10, 20, 13, 1);
	cage->floorGrid->setShader(shaderManager->getShader("grid"));
	cage->floorGrid->setCamera(Camera::mainCamera);
	cage->floorGrid->setPlayer(game->player);

	cage->wallGrid->setShader(shaderManager->getShader("wallGrid"));
	cage->wallGrid->setCamera(Camera::mainCamera);
	cage->wallGrid->setPlayer(game->player);

	boss= new BossEnemy(cage->centerWorldPosition + glm::vec3(0,3.f,0));
	game->add_entity<BossEnemy>(boss);
	boss->bossCage = cage;
}

void BossFightController::notifyBossDied() 
{

}

void BossFightController::cleanUp()
{
	delete cage;
	cage = nullptr;
	
	if (boss != nullptr)
		boss->destroy();
	boss = nullptr;

	hasInitialized = false;
}

void BossFightController::drawBossCage() {
	if (cage != nullptr)
		cage->draw();
}