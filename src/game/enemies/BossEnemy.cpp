#include "BossEnemy.h"
#include <iostream>

BossEnemy::BossEnemy(glm::vec3 position, MeshRenderer meshRenderer)
	: Enemy(position, meshRenderer)
{
	// setup nodes and connect them to behavior tree

	//behaviorTree = 
}

BossEnemy::~BossEnemy()
{
	
}

void BossEnemy::update(float deltaTime)
{
	behaviorTree.update(deltaTime);
}
