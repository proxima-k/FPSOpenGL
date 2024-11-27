#include "GrassAttackTask.h"

#include "BossEnemy.h"
#include "../grass_enemy/GrassEnemy.h"
#include <random>


void GrassAttackTask::onNodeStart(BT::Blackboard& blackboard)
{
	BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
	boss->setTargetColor(glm::vec3(0.3f, 1.f, 0.3f));

	cage = boss->bossCage;
	// resetValues();

	waveTimer = 0.f;
	currentWave = 0;
}

BT::NodeState GrassAttackTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{

	if (waveTimer <= 0.f) {
		if (currentWave >= MAX_WAVE)
			return BT::NodeState::SUCCESS;
		// spawn grass enemy algorithm
		// randomize a bunch of positions, check if the target cell is part of the positions, then skip it
		// set wait time, attack time and height offset for each grass enemy
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> randomHeight(0.3f, 0.5f);
		std::uniform_int_distribution<int> randomSpeedOffset(4, 9);
		
		glm::vec3 offset = cage->centerWorldPosition;
		for (int x = -cage->xCellCount; x <= cage->xCellCount - 1; x++) {
			for (int z = -cage->zCellCount; z <= cage->zCellCount - 1; z++) {

				glm::vec2 cellCenter = cage->floorGrid->getCellCenter(x, z);
				glm::vec3 spawnPos = glm::vec3(cellCenter.x + offset.x, 0, cellCenter.y + offset.z);
				GrassEnemy* newGrassEnemy = new GrassEnemy(spawnPos, warningTime, riseTime, attackTime, fallTime, randomHeight(gen), randomSpeedOffset(gen));
				game->add_entity(newGrassEnemy);
			}
		}

		waveTimer = warningTime + riseTime + attackTime + fallTime;
		currentWave++;
	}
	
	waveTimer -= deltaTime;

	return BT::NodeState::RUNNING;
}

void GrassAttackTask::onNodeFinish(BT::Blackboard& blackboard)
{
	BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
	boss->setTargetColor(glm::vec3(1.f));
}
