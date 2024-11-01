#pragma once

#include "../Enemy.h"
#include "../../engine/behavior_tree/BehaviorTree.h"

class BossEnemy : public Enemy
{
public:
	BossEnemy(glm::vec3 position, MeshRenderer meshRenderer);
	//~BossEnemy();

	void update(float deltaTime) override;

private:
	BT::BehaviorTree behaviorTree;
};

