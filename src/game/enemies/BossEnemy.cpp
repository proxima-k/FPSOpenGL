#include "BossEnemy.h"

BossEnemy::BossEnemy(glm::vec3 position, MeshRenderer meshRenderer)
	: Enemy(position, meshRenderer)
{
	// setup nodes and connect them to behavior tree

	//behaviorTree = 
}

void BossEnemy::update(float deltaTime)
{
	behaviorTree.update(deltaTime);
}
