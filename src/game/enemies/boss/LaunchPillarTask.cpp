#include "LaunchPillarTask.h"
#include "../../Game.h"
#include "../pillar_enemy/PillarEnemy.h"
#include "../../Player.h"
#include "../../Grid.h"

void LaunchPillarTask::onNodeStart(BT::Blackboard& blackboard)
{
    currentAttackCount = 0;
}

BT::NodeState LaunchPillarTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    if (currentAttackCount >= maxAttackCount)
        return BT::NodeState::SUCCESS;

    timer -= deltaTime;
    if (timer <= 0)
    {
        //game->spawn_entity<PillarEnemy>(glm::vec3(currentAttackCount + 2.5f, 0, currentAttackCount + 2.5f));

        // todo: make a simple algorithm to spawn the pillar in front of where the player is moving
        // maybe based on where the velocity is would be good.
		Player* player = blackboard.getValue<Player*>("player");
		if (player == nullptr)
			return BT::NodeState::FAILURE;
        if (floorGrid == nullptr)
			return BT::NodeState::FAILURE;

        glm::vec2 gridCellCenter = floorGrid->getCellCenter(player->transform.position);
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
}
