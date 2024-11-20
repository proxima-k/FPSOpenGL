#include "BossEnemy.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include <iostream>

#include <behavior_tree/nodes/Root.h>
#include <behavior_tree/nodes/SelectorNode.h>
#include <behavior_tree/nodes/SequenceNode.h>
#include <behavior_tree/nodes/RandomSelectorNode.h>
#include <behavior_tree/nodes/WaitTask.h>
#include "EnoughHealthDecorator.h"
#include "LaunchPillarTask.h"
#include "HealTask.h"

#include "BossBody.h"
#include "../../Game.h"
#include "../../Player.h"

BossEnemy::BossEnemy(glm::vec3 position)
	: Enemy(position)
{
	defaultPosition = position;

	collision_channel = Collision_Channel::None;

	// setup nodes for behavior tree
	BT::RootNode* root = new BT::RootNode();
	BT::SelectorNode* selector = new BT::SelectorNode();
	BT::SequenceNode* sequence = new BT::SequenceNode();
	BT::WaitTask* waitTask = new BT::WaitTask(2.6f, 0.6f);
	BT::RandomSelectorNode* randomSelector = new BT::RandomSelectorNode();
	EnoughHealthDecorator* enoughHealthDecorator = new EnoughHealthDecorator();
	HealTask* healTask = new HealTask();
	LaunchPillarTask* pillarAttackTask = new LaunchPillarTask();


	behaviorTree.setRootNode(root);
	root->setChild(selector);
	selector->addChild(enoughHealthDecorator);
	selector->addChild(healTask);
	enoughHealthDecorator->setChild(sequence);
	sequence->addChild(waitTask);
	sequence->addChild(randomSelector);
	// randomSelector->addChild(projectileTask);
	//randomSelector->addChild(pillarAttackTask);
	// randomSelector->addChild(spawnEnemiesTask);


	behaviorTree.getBlackboard().setValue<Player*>("player", game->player);
	behaviorTree.getBlackboard().setValue<BossEnemy*>("boss", this);
	behaviorTree.getBlackboard().setValue<int>("healAttempts", 3);

	maxHealth = 100;
	health = maxHealth;

	initMeshRenderer();
}

BossEnemy::~BossEnemy()
{
	
}

void BossEnemy::initMeshRenderer()
{
	int index = 0;
	for (int y = -1; y <= 1; y += 2) {
		float offset = 0.1f;
		float xAngle = 0.f;
		float zAngle = 90.f;
		for (int i = 0; i < 2; i++) {
			zAngle += i * 180.0f;
			for (int j = 0; j < 2; j++) {
				xAngle += j * 180.0f;

				float x = glm::cos(glm::radians(xAngle));
				float z = glm::sin(glm::radians(zAngle));

				//std::cout << x << " " << y << " " << z << std::endl;

				glm::vec3 spawnPosition(x / 2.0f + x * offset, y / 2.0f + y * offset, z / 2.0f + z * offset);

				glm::vec3 offsetDirection = glm::normalize(spawnPosition);
				float offsetMagnitude = glm::sqrt(3 * offset * offset);

				std::cout << offsetDirection.x << " " << offsetDirection.y << " " << offsetDirection.z << std::endl;

				BossBody* body = new BossBody(spawnPosition, index, offsetDirection, offsetMagnitude);
				body->setBossController(this);
				game->add_entity(body);

				bossBodies[index] = body;
				index++;
			}
		}
	}

	currentBodyCount = 8;
	
	transform.scale = glm::vec3(1);
}

void BossEnemy::update(float deltaTime)
{
	behaviorTree.update(deltaTime);

	timeElapsed += deltaTime;

	float heightOffset = glm::sin(timeElapsed * 2) / 5;
	transform.position = defaultPosition + glm::vec3(0.f, heightOffset, 0.f);

	//std::cout << "boss is alive" << std::endl;
}

void BossEnemy::notifyBossBodyDeath(int index)
{
	bossBodies[index] = nullptr;
	currentBodyCount--;

	if (currentBodyCount <= 0) {
		// add extra stuff to show boss died
		destroy();
	}
}

void BossEnemy::setCanCollide(bool canCollide)
{
	for (int i = 0; i < bossBodies.size(); i++) {
		if (bossBodies[i] == nullptr) continue;

		if (canCollide)
			bossBodies[i]->collision_channel = Collision_Channel::Enemy;
		else
			bossBodies[i]->collision_channel = Collision_Channel::None;
	}
}
