#include "BossEnemy.h"

#include <iostream>
#include <behavior_tree/nodes/Root.h>
#include <MeshManager.h>
#include <ShaderManager.h>

#include "LaunchPillarTask.h"


BossEnemy::BossEnemy(glm::vec3 position)
	: Enemy(position)
{
	// setup nodes for behavior tree
	BT::RootNode* root = new BT::RootNode();
	LaunchPillarTask* pillarAttackTask = new LaunchPillarTask();

	behaviorTree.setRootNode(root);
	root->setChild(pillarAttackTask);

	// todo: change this so that pillar doesn't require a mesh renderer stored within the blackboard
	//behaviorTree.getBlackboard().setValue<MeshRenderer*>("pillarMeshRenderer", &meshRenderer);
	initMeshRenderer();
}

BossEnemy::~BossEnemy()
{
	
}

void BossEnemy::initMeshRenderer()
{
	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
	meshRenderer->setColor(glm::vec3(0.3f, 0.3f, 1.f));
}
