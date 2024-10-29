#pragma once

#include "../Enemy.h"
#include "../behavior_tree/BehaviorTree.h"

class BossEnemy : public Enemy
{
public:
	BossEnemy();
	~BossEnemy();

	void update(float deltaTime) override;

private:
	BT::BehaviorTree behaviorTree;
};

