#include "BossBody.h"
#include "BossEnemy.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include <behavior_tree/nodes/Root.h>
#include <behavior_tree/nodes/WaitTask.h>
#include <behavior_tree/nodes/SequenceNode.h>
#include "BodyRotateTask.h"

BossBody::BossBody(glm::vec3 spawnPosition)
	: Enemy(spawnPosition)
{
	BT::RootNode* rootNode = new BT::RootNode();
	BT::SequenceNode* sequenceNode = new BT::SequenceNode();
	BodyRotateTask* bodyRotateTask = new BodyRotateTask();
	BT::WaitTask* waitTask = new BT::WaitTask(3.f, 1.2f);

	behaviorTree.setRootNode(rootNode);
	rootNode->setChild(sequenceNode);
	sequenceNode->addChild(waitTask);
	sequenceNode->addChild(bodyRotateTask);

	behaviorTree.getBlackboard().setValue<Entity*>("body", this);

	maxHealth = 500;
	health = maxHealth;

	transform.scale = glm::vec3(1);

	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	meshRenderer->setColor(glm::vec3(0.3f, 0.3f, 1.f));
}

BossBody::~BossBody()
{
}

void BossBody::take_damage(int damage)
{
	health -= damage;
	if (health <= 0)
	{
		die(1.f);
	}
}

void BossBody::die(float xpAmount)
{
	//bossController->body died
	destroy();
}

void BossBody::setBossController(BossEnemy* bossController)
{
}
