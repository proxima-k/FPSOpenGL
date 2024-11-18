#pragma once


#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>

class BossEnemy; // forward declaration to prevent circular dependencies

class BossBody : public Enemy {
public:
	BossBody(glm::vec3 spawnPosition);
	BossBody(glm::vec3 spawnPosition, int index, glm::vec3 offsetDirection, float offset);
	~BossBody();

	void update(float deltaTime) override { behaviorTree.update(deltaTime); updateTransform(); }
	void setBossController(BossEnemy* bossController);
	
	
	void take_damage(int damage) override;
	void die(float xpAmount) override;
	void heal(int amount);
	
	// updates body size and position based on health
	void updateTransform();

	// used to indicate which index in a vector on the BossEnemy script
	int index = -1;							 
	// reference to the main controller
	BossEnemy* bossController = nullptr;

private:
	int sizeSteps = 8;
	glm::vec3 offsetDirection;
	float offset;

	BT::BehaviorTree behaviorTree;
};