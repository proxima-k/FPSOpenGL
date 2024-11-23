#include "BossHealingCube.h"

#include "../../Game.h"
#include <MeshManager.h>
#include <ShaderManager.h>
#include "BossBody.h"
#include <glm/vec3.hpp>

#include <behavior_tree/nodes/Root.h>
#include "HealCubeEntranceTask.h"

BossHealingCube::BossHealingCube(glm::vec3 spawnPosition, BossBody* bossBodyToHeal, HealTask* healTaskNode, int index, glm::vec3 cellNormal)
	: Enemy(spawnPosition), bossBodyToHeal(bossBodyToHeal), healTaskNode(healTaskNode), index(index), faceDirection(cellNormal), cellPosition(spawnPosition)
{
	BT::RootNode* root = new BT::RootNode();
	HealCubeEntranceTask* entranceTask = new HealCubeEntranceTask(index/3.5f);
	entranceTask->canRepeat = false;

	behaviorTree.setRootNode(root);
	root->setChild(entranceTask);

	/*transform.position = cellPosition + (cellNormal / 2.f);
	transform.scale = glm::vec3(1.f);*/
	transform.scale = glm::vec3(0.f);


	behaviorTree.getBlackboard().setValue<HealTask*>("healTaskNode", healTaskNode);
	behaviorTree.getBlackboard().setValue<glm::vec3>("cellNormal", cellNormal);
	behaviorTree.getBlackboard().setValue<glm::vec3>("cellPosition", cellPosition);
	behaviorTree.getBlackboard().setValue<BossHealingCube*>("healCube", this);

	shader = shaderManager->getShader("healcube");
	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shader, Camera::mainCamera);
	meshRenderer->setColor(glm::vec3(0.7f));
}

BossHealingCube::~BossHealingCube()
{
	if (healingLine != nullptr)
		healingLine->destroy();
}

void BossHealingCube::update(float deltaTime)
{
	behaviorTree.update(deltaTime);
	if (healingLine != nullptr)
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

void BossHealingCube::spawnHealingLine()
{
	healingLine = game->spawn_healing_line(cellPosition + faceDirection, bossBodyToHeal->transform.position);

}
