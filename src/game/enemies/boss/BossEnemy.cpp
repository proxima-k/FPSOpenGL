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
#include "WaveAttackTask.h"
#include "HealTask.h"

#include "BossBody.h"
#include "../../Game.h"
#include "../../Player.h"

#include <random>

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
	WaveAttackTask* waveAttackTask = new WaveAttackTask();

	behaviorTree.setRootNode(root);
	root->setChild(selector);
	selector->addChild(enoughHealthDecorator);
	selector->addChild(healTask);
	enoughHealthDecorator->setChild(sequence);
	sequence->addChild(waitTask);
	sequence->addChild(randomSelector);
	randomSelector->addChild(waveAttackTask);
	randomSelector->addChild(pillarAttackTask);
	// randomSelector->addChild(projectileTask);
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
		float xAngle = 0.f;
		float zAngle = 90.f;
		for (int i = 0; i < 2; i++) {
			zAngle += i * 180.0f;
			for (int j = 0; j < 2; j++) {
				xAngle += j * 180.0f;

				float x = glm::cos(glm::radians(xAngle));
				float z = glm::sin(glm::radians(zAngle));

				glm::vec3 spawnPosition(x / 2.0f + x * bodyOffset, y / 2.0f + y * bodyOffset, z / 2.0f + z * bodyOffset);

				glm::vec3 offsetDirection = glm::normalize(spawnPosition);
				

				std::cout << offsetDirection.x << " " << offsetDirection.y << " " << offsetDirection.z << std::endl;

				BossBody* body = new BossBody(spawnPosition, index, offsetDirection, getOffsetMagnitude());
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

	setBodyColor(glm::vec3(glm::sin(timeElapsed * 2) / 2 + 0.7f, 0.2f, 0.2f));
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

void BossEnemy::notifyBossBodyRegenerate(BossBody* body, int index)
{
	bossBodies[index] = body;
	currentBodyCount++;
}

int BossEnemy::getRandomEmptyIndex()
{

	int emptySlots = 8 - currentBodyCount;
	if (emptySlots < 1) return -1;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> randomSteps(1, emptySlots);
	int steps = randomSteps(gen);
	
	for (int i = 0; i < bossBodies.size(); i++) {
		if (bossBodies[i] == nullptr)
			steps--;

		if (steps <= 0) 
			return i;
	}

	return -1;
}

float BossEnemy::getOffsetMagnitude()
{
	return glm::sqrt(3 * bodyOffset * bodyOffset);
}

glm::vec3 BossEnemy::getOffsetDirectionFromIndex(int index)
{
	int currentIndex = 0;
	for (int y = -1; y <= 1; y += 2) {
		float xAngle = 0.f;
		float zAngle = 90.f;
		for (int i = 0; i < 2; i++) {
			zAngle += i * 180.0f;
			for (int j = 0; j < 2; j++) {
				xAngle += j * 180.0f;

				float x = glm::cos(glm::radians(xAngle));
				float z = glm::sin(glm::radians(zAngle));

				glm::vec3 spawnPosition(x / 2.0f + x * bodyOffset, y / 2.0f + y * bodyOffset, z / 2.0f + z * bodyOffset);

				glm::vec3 offsetDirection = glm::normalize(spawnPosition);

				if (currentIndex == index)
					return offsetDirection;

				currentIndex++;
			}
		}
	}

	std::cout << "Error: get offset direction from index" << std::endl;
	return glm::vec3(0.f);
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

void BossEnemy::setBodyColor(glm::vec3 color)
{
	for (int i = 0; i < bossBodies.size(); i++) {
		if (bossBodies[i] == nullptr) continue;

		bossBodies[i]->meshRenderer->setColor(color);
	}
}
