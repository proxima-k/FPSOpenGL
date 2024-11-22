#pragma once

#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>
#include "HealTask.h"

#include "HealingLine.h"

class BossBody;

class BossHealingCube : public Enemy
{
public:
	BossHealingCube(glm::vec3 spawnPosition, BossBody* bossBodyToHeal, HealTask* healTaskNode, int index, glm::vec3 cellNormal);
	~BossHealingCube();

	// update ray start position
	// update ray end position
	// shader for ray

	void update(float deltaTime) override;
	void draw() override;

	void die(float xpSpawnAmount) override
	{
		if (healTaskNode != nullptr)
			healTaskNode->notifyHealingCubeDeath(index);
		
		game->spawn_particle_source(transform.position, 40.0f, 0.3f, 2.0f);
		destroy();
	}

private:
	HealTask* healTaskNode = nullptr;
	HealingLine* healingLine = nullptr;
	BossBody* bossBodyToHeal = nullptr;
	
	BT::BehaviorTree behaviorTree;


	Shader* shader;
	glm::vec3 cellPosition;
	glm::vec3 faceDirection;
	int index;
};