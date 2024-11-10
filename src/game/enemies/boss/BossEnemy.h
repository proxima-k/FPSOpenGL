#pragma once

#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>
#include <glm/gtc/quaternion.hpp>

class BossBody;

class BossEnemy : public Enemy
{
public:
	BossEnemy(glm::vec3 position);
	~BossEnemy();

	void initMeshRenderer() override;
	void update(float deltaTime) override;


private:
	std::vector<BossBody*> bossBodies;

	BT::BehaviorTree behaviorTree;
};