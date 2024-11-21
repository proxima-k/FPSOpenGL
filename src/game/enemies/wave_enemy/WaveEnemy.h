#pragma once

#include "../../Entity.h"

#include <behavior_tree/BehaviorTree.h>

class WaveEnemy : public Entity {
public:
	WaveEnemy(glm::vec3 position);
	void update(float deltaTime) override { behaviorTree.update(deltaTime); }

private:
	BT::BehaviorTree behaviorTree;
};