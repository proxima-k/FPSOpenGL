
#pragma once

#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>
#include <glm/gtc/quaternion.hpp>
#include <vector>

class BossBody;

class BossEnemy : public Enemy
{
public:
	BossEnemy(glm::vec3 position);
	~BossEnemy();

	void initMeshRenderer() override;
	void update(float deltaTime) override;

	void NotifyBossBodyDeath(int index);

	glm::vec3 defaultPosition;

private:
	std::vector<BossBody*> bossBodies = std::vector<BossBody*>(8, nullptr);
	BT::BehaviorTree behaviorTree;

	int currentBodyCount = 0;
	float timeElapsed = 0;
};