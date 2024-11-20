#include "BossHealingCube.h"

#include "../../Game.h"
#include <MeshManager.h>
#include <ShaderManager.h>

#include "BossBody.h"

BossHealingCube::BossHealingCube(glm::vec3 spawnPosition, BossBody* bossBodyToHeal, HealTask* healTaskNode, int index)
	: Enemy(spawnPosition), bossBodyToHeal(bossBodyToHeal), healTaskNode(healTaskNode), index(index)
{
	healingLine = game->spawn_healing_line(spawnPosition, bossBodyToHeal->transform.position);

	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	meshRenderer->setColor(glm::vec3(0.7f));
	transform.scale = glm::vec3(1.f);
}

BossHealingCube::~BossHealingCube()
{
	healingLine->destroy();
}

void BossHealingCube::update(float deltaTime)
{
	healingLine->updateEndPosition(bossBodyToHeal->transform.position);
}
