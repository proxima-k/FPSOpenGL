#include "BossEnemy.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include <iostream>

#include <behavior_tree/nodes/Root.h>
#include "LaunchPillarTask.h"
#include <behavior_tree/nodes/WaitTask.h>

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
	//LaunchPillarTask* pillarAttackTask = new LaunchPillarTask();

	behaviorTree.setRootNode(root);
	//root->setChild(pillarAttackTask);

	behaviorTree.getBlackboard().setValue<Player*>("player", game->player);

	maxHealth = 100;
	health = maxHealth;

	//meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	//meshRenderer->setColor(glm::vec3(0.3f, 0.3f, 1.f));

	// todo: change this so that pillar doesn't require a mesh renderer stored within the blackboard
	//behaviorTree.getBlackboard().setValue<MeshRenderer*>("pillarMeshRenderer", &meshRenderer);
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

	std::cout << "boss is alive" << std::endl;
}

void BossEnemy::NotifyBossBodyDeath(int index)
{
	bossBodies[index] = nullptr;
	currentBodyCount--;

	if (currentBodyCount <= 0) {
		// add extra stuff to show boss died
		destroy();
	}
}
