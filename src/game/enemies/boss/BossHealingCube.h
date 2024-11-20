#pragma once

#include "../../Enemy.h"
#include <behavior_tree/BehaviorTree.h>
#include "HealTask.h"

#include "HealingLine.h"

class BossHealingCube : public Enemy
{
public:
	BossHealingCube(glm::vec3 spawnPosition, glm::vec3 targetPosition, HealTask* healTaskNode, int index);
	~BossHealingCube();
	// target position

	// update ray start position
	// update ray end position
	// shader for ray

	void die(float xpSpawnAmount) override
	{
		if (healTaskNode != nullptr) {
			healTaskNode->notifyHealingCubeDeath(index);
		}


		game->spawn_particle_source(transform.position, 40.0f, 0.3f, 2.0f);
		destroy();
	}

private:
	HealTask* healTaskNode = nullptr;
	HealingLine* healingLine = nullptr;
	
	BT::BehaviorTree behaviorTree;

	int index;
};