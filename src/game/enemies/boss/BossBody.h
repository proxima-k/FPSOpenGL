#pragma once


#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>

class BossEnemy; // forward declaration to prevent circular dependencies

class BossBody : public Enemy {
public:
	BossBody(glm::vec3 spawnPosition);
	~BossBody();

	void update(float deltaTime) override { behaviorTree.update(deltaTime); }
	void die(float xpAmount) override;
	void setBossController(BossEnemy* bossController);

	int index = -1;							 // used to indicate which index in a vector on the BossEnemy script
	BossEnemy* bossController = nullptr;	 // reference to the main controller
private:
	BT::BehaviorTree behaviorTree;
};