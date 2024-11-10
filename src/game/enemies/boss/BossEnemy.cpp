#include "BossEnemy.h"

#include <iostream>
#include <behavior_tree/nodes/Root.h>
#include <MeshManager.h>
#include <ShaderManager.h>

#include "LaunchPillarTask.h"

#include "BossBody.h"


BossEnemy::BossEnemy(glm::vec3 position)
	: Enemy(position)
{
	collision_channel = Collision_Channel::None;

	// setup nodes for behavior tree
	BT::RootNode* root = new BT::RootNode();
	LaunchPillarTask* pillarAttackTask = new LaunchPillarTask();

	behaviorTree.setRootNode(root);
	root->setChild(pillarAttackTask);

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
	//BossBody* body = game->spawn_entity<BossBody>(glm::vec3(0, 1, 4));
	//return;

	for (int z = 1; z >= -1; z -= 2) {
		float offset = 0.1f;
		float yAngle = 90.f;
		float xAngle = 0.f;
		for (int i = 0; i < 2; i++) {
			yAngle += i * 180.0f;
			for (int j = 0; j < 2; j++) {
				xAngle += j * 180.0f;

				float y = glm::sin(glm::radians(yAngle));
				float x = glm::cos(glm::radians(xAngle));

				glm::vec3 spawnPosition(x / 2.0f + x * offset, y / 2.0f + y * offset, z / 2.0f + z * offset);

				BossBody* body = game->spawn_entity<BossBody>(spawnPosition);

				std::cout << x << " " << y << " " << z << std::endl;

				bossBodies.push_back(body);
			}
		}
	}
	

	transform.scale = glm::vec3(1);
	//meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	//meshRenderer->setColor(glm::vec3(0.3f, 0.3f, 1.f));
}

void BossEnemy::update(float deltaTime)
{
	behaviorTree.update(deltaTime);

	//for (int i=4)
}
