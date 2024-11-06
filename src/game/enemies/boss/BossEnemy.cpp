#include "BossEnemy.h"

#include <iostream>
#include <behavior_tree/nodes/Root.h>

#include "LaunchPillarTask.h"

BossEnemy::BossEnemy(glm::vec3 position, MeshRenderer meshRenderer)
	: Enemy(position, meshRenderer)
{
	// setup nodes for behavior tree
	BT::RootNode* root = new BT::RootNode();
	LaunchPillarTask* pillarAttackTask = new LaunchPillarTask();

	behaviorTree.setRootNode(root);
	root->setChild(pillarAttackTask);

	behaviorTree.getBlackboard().setValue<MeshRenderer*>("pillarMeshRenderer", &meshRenderer);
}

BossEnemy::~BossEnemy()
{
	
}
