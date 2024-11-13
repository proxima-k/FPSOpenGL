#pragma once

#include "../../Enemy.h"
#include "behavior_tree/BehaviorTree.h"

class BossHealingCube : public Enemy
{
public:
	BossHealingCube(glm::vec3 spawnPosition, glm::vec3 targetPosition);
	~BossHealingCube();
	// target position
	// create two points that should be handled by the drawing in this class

	// update ray start position
	// update ray end position
	// shader for ray
	// 

private:
	BT::BehaviorTree behaviorTree;
};