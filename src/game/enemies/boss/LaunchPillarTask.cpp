#include "LaunchPillarTask.h"
#include "../../Game.h"
#include "../pillar_enemy/PillarEnemy.h"
#include "../../Player.h"
#include "../../Grid.h"
#include "BossEnemy.h"
#include "BossCage.h"


void LaunchPillarTask::onNodeStart(BT::Blackboard& blackboard)
{
    currentAttackCount = 0;
    
    BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
    boss->setTargetColor(glm::vec3(1.f, 0.3f, 0.3f));
}

BT::NodeState LaunchPillarTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    if (currentAttackCount >= maxAttackCount)
        return BT::NodeState::SUCCESS;

    timer -= deltaTime;
    if (timer <= 0)
    {
        BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");

		Player* player = blackboard.getValue<Player*>("player");
		if (player == nullptr)
			return BT::NodeState::FAILURE;

        BossCage* cage = boss->bossCage;
        Grid* floorGrid = cage->floorGrid;
        float constant = 3.5f;
        glm::vec3 predictPosition = player->transform.position + glm::normalize(player->physicsbody.velocity) * constant;

        glm::vec2 cellIndex = floorGrid->getCellCoords(predictPosition - cage->centerWorldPosition);
        //std::cout << cellIndex.x << "   " << cellIndex.y << std::endl;
        
        int xCellCount = cage->xCellCount;
        int zCellCount = cage->zCellCount;
        cellIndex.x = glm::clamp((int)cellIndex.x, -xCellCount, xCellCount - 1);
        cellIndex.y = glm::clamp((int)cellIndex.y, -zCellCount, zCellCount - 1);

        glm::vec2 gridCellCenter = floorGrid->getCellCenter(cellIndex.x, cellIndex.y);
        glm::vec3 spawnPosition = glm::vec3(gridCellCenter.x, 0, gridCellCenter.y) + cage->centerWorldPosition;
		spawnPosition.y = 0;
        game->spawn_entity<PillarEnemy>(spawnPosition);

        currentAttackCount++;
        timer = attackCooldown;
    }

    return BT::NodeState::RUNNING;
}

void LaunchPillarTask::onNodeFinish(BT::Blackboard& blackboard)
{
    BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
    boss->setTargetColor(glm::vec3(1.f));
}
