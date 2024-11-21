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
        if (floorGrid == nullptr)
			return BT::NodeState::FAILURE;

        float constant = 3.5f;
        glm::vec2 gridCellCenter = boss->bossCage->floorGrid->getCellCenter(player->transform.position + glm::normalize(player->physicsbody.velocity) * constant);
        glm::vec3 spawnPosition = glm::vec3(gridCellCenter.x, 0, gridCellCenter.y);
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
