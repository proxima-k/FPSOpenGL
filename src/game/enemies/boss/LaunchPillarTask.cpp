#include "LaunchPillarTask.h"
#include "../../Game.h"
#include "../pillar_enemy/PillarEnemy.h"


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
        //MeshRenderer* pillarMeshRenderer = blackboard.getValue<MeshRenderer*>("pillarMeshRenderer");
        //if (pillarMeshRenderer == nullptr)
            //return BT::NodeState::FAILURE;

        // todo: make a simple algorithm to spawn the pillar in front of where the player is moving
        // maybe based on where the velocity is would be good.
        game->spawn_entity<PillarEnemy>(glm::vec3(currentAttackCount + 2.5f, 0, currentAttackCount + 2.5f));

        currentAttackCount++;
        timer = attackCooldown;
    }

    return BT::NodeState::RUNNING;
}

void LaunchPillarTask::onNodeFinish(BT::Blackboard& blackboard)
{
}
