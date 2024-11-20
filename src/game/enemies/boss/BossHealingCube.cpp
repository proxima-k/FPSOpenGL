#include "BossHealingCube.h"

#include "../../Game.h"
#include <MeshManager.h>
#include <ShaderManager.h>


BossHealingCube::BossHealingCube(glm::vec3 spawnPosition, glm::vec3 targetPosition, HealTask* healTaskNode, int index)
	: Enemy(spawnPosition), healTaskNode(healTaskNode), index(index)
{
	healingLine = game->spawn_healing_line(spawnPosition, targetPosition);

	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	meshRenderer->setColor(glm::vec3(0.7f));
	transform.scale = glm::vec3(1.f);
}

BossHealingCube::~BossHealingCube()
{
	healingLine->destroy();
}