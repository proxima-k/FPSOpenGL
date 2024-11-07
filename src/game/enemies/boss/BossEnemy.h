#pragma once

#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>

class BossEnemy : public Enemy
{
public:
	BossEnemy(glm::vec3 position);
	~BossEnemy();

	void update(float deltaTime) override { behaviorTree.update(deltaTime); }

private:
	BT::BehaviorTree behaviorTree;
};