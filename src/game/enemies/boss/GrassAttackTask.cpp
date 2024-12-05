#include "GrassAttackTask.h"

#include "BossEnemy.h"
#include "../grass_enemy/GrassEnemy.h"
#include <random>
#include <vector>
#include <map>
#include <utility>


void GrassAttackTask::onNodeStart(BT::Blackboard& blackboard)
{
	BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
	boss->setTargetColor(glm::vec3(0.5f, 0.99f, 0.f));

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

		game->camera->invokeScreenShake(.045f, warningTime - 0.75f);

		// for when the grass rises
		hasShakedCamera = false;
		shakeCountdown = warningTime;

		// spawn grass enemy algorithm
		// randomize a bunch of positions, check if the target cell is part of the positions, then skip it
		// set wait time, attack time and height offset for each grass enemy
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> randomHeight(0.3f, 0.5f);

		int xCellCount = cage->xCellCount;
		int zCellCount = cage->zCellCount;
		std::uniform_int_distribution<int> randomXCell(-xCellCount, xCellCount - 1);
		std::uniform_int_distribution<int> randomZCell(-zCellCount, zCellCount - 1);
		std::map<std::pair<int, int>, bool> emptyCells;
		int currentEmptyCellCount = 3 - currentWave;
		for (int i = 0; i < currentEmptyCellCount; i++) {
			int xCell = randomXCell(gen);
			int zCell = randomZCell(gen);

			emptyCells[{xCell, zCell}] = true;
			emptyCells[{xCell, zCell-1}] = true;
			emptyCells[{xCell, zCell+1}] = true;
			emptyCells[{xCell+1, zCell}] = true;
			emptyCells[{xCell+1, zCell-1}] = true;
			emptyCells[{xCell+1, zCell+1}] = true;
			emptyCells[{xCell-1, zCell}] = true;
			emptyCells[{xCell-1, zCell-1}] = true;
			emptyCells[{xCell-1, zCell+1}] = true;
		}
		
		glm::vec3 offset = cage->centerWorldPosition;
		for (int x = -cage->xCellCount; x <= cage->xCellCount - 1; x++) {
			for (int z = -cage->zCellCount; z <= cage->zCellCount - 1; z++) {
				if (emptyCells.find({ x,z }) != emptyCells.end())
					continue;

				glm::vec2 cellCenter = cage->floorGrid->getCellCenter(x, z);
				glm::vec3 spawnPos = glm::vec3(cellCenter.x + offset.x, 0, cellCenter.y + offset.z);
				GrassEnemy* newGrassEnemy = new GrassEnemy(spawnPos, warningTime, riseTime, attackTime, fallTime, randomHeight(gen), 8.f);
				game->add_entity(newGrassEnemy);
				
			}
		}

		/*glm::vec2 cellCenter = cage->floorGrid->getCellCenter(0, 0);
		glm::vec3 spawnPos = glm::vec3(cellCenter.x + offset.x, 0, cellCenter.y + offset.z);
		GrassEnemy* newGrassEnemy = new GrassEnemy(spawnPos, warningTime, riseTime, attackTime, fallTime, randomHeight(gen), randomSpeedOffset(gen));
		game->add_entity(newGrassEnemy);*/

		waveTimer = warningTime + riseTime + attackTime + fallTime + 0.5f;
		currentWave++;
	}

	shakeCountdown -= deltaTime;
	if (!hasShakedCamera && shakeCountdown <= 0.f) {
		game->camera->invokeScreenShake(0.2f, 0.3f);
		hasShakedCamera = true;
	}
	
	waveTimer -= deltaTime;

	return BT::NodeState::RUNNING;
}

void GrassAttackTask::onNodeFinish(BT::Blackboard& blackboard)
{
	BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
	boss->setTargetColor(glm::vec3(1.f));
}
