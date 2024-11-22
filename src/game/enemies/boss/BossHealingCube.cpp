#include "BossHealingCube.h"

#include "../../Game.h"
#include <MeshManager.h>
#include <ShaderManager.h>

#include "BossBody.h"

BossHealingCube::BossHealingCube(glm::vec3 spawnPosition, BossBody* bossBodyToHeal, HealTask* healTaskNode, int index, glm::vec3 cellNormal)
	: Enemy(spawnPosition), bossBodyToHeal(bossBodyToHeal), healTaskNode(healTaskNode), index(index), faceDirection(cellNormal), cellPosition(spawnPosition)
{
	transform.position = cellPosition + (cellNormal / 2.f);

	healingLine = game->spawn_healing_line(cellPosition + cellNormal, bossBodyToHeal->transform.position);
	shader = shaderManager->getShader("healcube");


	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shader, Camera::mainCamera);
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

void BossHealingCube::draw()
{
	if (meshRenderer != nullptr) {
		meshRenderer->setColor(healTaskNode->newBossBody->meshRenderer->getColor());
		shader->Bind();

		glm::vec3 thresholdPos = cellPosition + faceDirection * healTaskNode->getHealPercentage();
		shader->SetFloat3("u_thresholdPos", thresholdPos);
		shader->SetFloat3("u_cellNormal", faceDirection);
	}
	Entity::draw();
}
