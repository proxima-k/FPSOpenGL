#pragma once

#include "../../Entity.h"

#include <behavior_tree/BehaviorTree.h>

class GrassEnemy : public Entity {
public:
	GrassEnemy(glm::vec3 position, float warningTime, float riseTime, float attackTime, float fallTime, float maxHeight, float speedOffset);

	void update(float deltaTime) override { behaviorTree.update(deltaTime); }

private:
	BT::BehaviorTree behaviorTree;
};