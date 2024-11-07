#pragma once

#include "../../Game.h"
#include "../../Entity.h"
#include "../../Physicsbody.h"
#include "../../xp/XP.h"

#include <behavior_tree/BehaviorTree.h>

class PillarEnemy : public Entity
{
public:
	PillarEnemy(glm::vec3 position);

	void update(float deltaTime) override { behaviorTree.update(deltaTime); }
	void initMeshRenderer() override;
	//void updateHeight();

private:
	//float currentHeight;
	//float waitTimer = 5.0f;
	BT::BehaviorTree behaviorTree;
};