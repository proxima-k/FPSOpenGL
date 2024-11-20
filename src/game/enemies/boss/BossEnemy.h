
#pragma once

#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>
#include <glm/gtc/quaternion.hpp>
#include <vector>

#include "BossCage.h"

class BossBody;

class BossEnemy : public Enemy
{
public:
	BossEnemy(glm::vec3 position);
	~BossEnemy();

	void initMeshRenderer() override;
	void update(float deltaTime) override;

	void notifyBossBodyDeath(int index);

	glm::vec3 defaultPosition;
	int getBodyCount() { return currentBodyCount; }

	BossCage* bossCage = nullptr;

	void setCanCollide(bool canCollide);

private:
	std::vector<BossBody*> bossBodies = std::vector<BossBody*>(8, nullptr);
	BT::BehaviorTree behaviorTree;

	int currentBodyCount = 0;
	float timeElapsed = 0;
};