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

	//maxHealth = 60;
	health = MAX_HEALTH;
	maxHealth = MAX_HEALTH;

	transform.scale = glm::vec3(1);

	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	meshRenderer->setColor(glm::vec3(0.3f, 0.3f, 1.f));
}

BossBody::BossBody(glm::vec3 spawnPosition, int _index, glm::vec3 _offsetDirection, float _offsetMagnitude)
	: BossBody(spawnPosition)
{
	index = _index;
	offsetDirection = _offsetDirection;
	offsetMagnitude = _offsetMagnitude;
	// get Y cord
	// find range
	// check that index within the range to find which quarter it belongs to

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
	updateTransform();
}

void BossBody::heal(int amount) {
	health += amount;
	if (health > maxHealth)
		health = maxHealth;

	updateTransform();
}

void BossBody::die(float xpAmount)
{
	//bossController->body died
	// notify boss brain that this body died
	destroy();
	bossController->notifyBossBodyDeath(index);
}

void BossBody::setBossController(BossEnemy* _bossController)
{
	bossController = _bossController;
}

void BossBody::updateTransform()
{
	float healthPercentage = health / (float)maxHealth;
	
	
	float size;
	if (smoothSizing) {
		size = healthPercentage;
	}
	else {
		float scale = 1.f * (1.f / sizeSteps);
		int step = glm::ceil(healthPercentage * sizeSteps);
		size = step * scale;
	}

	transform.scale = glm::vec3(size);
	// have to get distance from center of cube the a corner
	glm::vec3 bossPosition = bossController->transform.position;

	transform.position = bossPosition + offsetDirection * (glm::sqrt(3 * size * size / 4) + offsetMagnitude);
}