
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
	void notifyBossBodyRegenerate(BossBody* body, int index);
	int getRandomEmptyIndex();
	glm::vec3 getOffsetDirectionFromIndex(int index);
	float getOffsetMagnitude();

	int getBodyCount() { return currentBodyCount; }
	void setCanCollide(bool canCollide);
	void setTargetColor(glm::vec3 color);

	glm::vec3 defaultPosition;
	BossCage* bossCage = nullptr;

	std::vector<BossBody*> bossBodies = std::vector<BossBody*>(8, nullptr);

private:
	BT::BehaviorTree behaviorTree;

	int currentBodyCount = 0;
	float timeElapsed = 0;
	float bodyOffset = 0.1f;

	float colorTValue = 0;
	glm::vec3 targetColor = glm::vec3(1);
	glm::vec3 currentColor = glm::vec3(0);
	glm::vec3 startColor = glm::vec3(0);

	void setBodyColor(glm::vec3 color);
};