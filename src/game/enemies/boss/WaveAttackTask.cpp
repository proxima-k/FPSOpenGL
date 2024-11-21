#include "WaveAttackTask.h"

#include "BossCage.h"
#include "BossEnemy.h"
#include "../../Game.h"
#include "../wave_enemy/WaveEnemy.h"

void WaveAttackTask::onNodeStart(BT::Blackboard& blackboard)
{
	BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
	boss->setTargetColor(glm::vec3(0.3f, 0.3f, 1.f));

	std::cout << "starting wave attack" << std::endl;
	cage = boss->bossCage;
	resetValues();

	timer = timeToSpawnWave;
	loopsLeft = totalLoops;
}

BT::NodeState WaveAttackTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	// handle loop logic first

	if (loopsLeft <= 0) 
		return BT::NodeState::SUCCESS;


	if (loopTimer > 0) {
		loopTimer -= deltaTime;
		return BT::NodeState::RUNNING;
	}
	// cannot perform wave stuff if loop timer is larger than 0


	// if loop timer is 0, then summon till steps left is 0
	// when steps left is 0, reset loop stuff and decrease loops left by 1
	if (stepsLeft <= 0) {
		loopsLeft--;
		loopTimer = timeToSpawnLoop;
		resetValues();
	}

	if (timer <= 0) {
		//std::cout << "WAVE ATTACK" << std::endl;
		// reset timer
		timer = timeToSpawnWave;
		
		//std::cout << currentXCellCount << std::endl;
		//std::cout << currentZCellCount << std::endl;
		for (int x = -currentXCellCount; x <= currentXCellCount - 1; x += currentXCellCount * 2 - 1) {
			for (int z = -currentZCellCount; z <= currentZCellCount - 1; z++) {
				// offset x and z cell index
				glm::vec2 cellCenter = cage->floorGrid->getCellCenter(x, z);
				game->spawn_entity<WaveEnemy>(glm::vec3(cellCenter.x, 0, cellCenter.y));
			}
		}

		for (int z = -currentZCellCount; z <= currentZCellCount - 1; z += currentZCellCount * 2 - 1) {
			for (int x = -currentXCellCount+1; x <= currentXCellCount-2; x++) {
				glm::vec2 cellCenter = cage->floorGrid->getCellCenter(x, z);
				game->spawn_entity<WaveEnemy>(glm::vec3(cellCenter.x, 0, cellCenter.y));
			}
		}

		stepsLeft--;
		currentXCellCount++;
		currentZCellCount++;
	}

	timer -= deltaTime;

	return BT::NodeState::RUNNING;
}

void WaveAttackTask::onNodeFinish(BT::Blackboard& blackboard)
{
	BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
	boss->setTargetColor(glm::vec3(1.f));
}

void WaveAttackTask::resetValues()
{
	// get boss cage x and z cell count
	int xCount = cage->xCellCount;
	int zCount = cage->zCellCount;

	stepsLeft = glm::min(cage->xCellCount, cage->zCellCount);
	
	// larger one gets minus by steps left + 1
	if (xCount > zCount) {
		currentXCellCount = xCount - stepsLeft + 1;
		currentZCellCount = 1;
	}
	else {
		currentXCellCount = 1;
		currentZCellCount = zCount - stepsLeft + 1;
	}
}