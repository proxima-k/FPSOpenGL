#include "HealTask.h"

#include "../../Game.h"
#include "BossCage.h"
#include "BossHealingCube.h"

void HealTask::onNodeStart(BT::Blackboard& blackboard)
{
    // get random cell on the wall grid, spawn a cube
    // assign cube's target location
    // form a line towards the target location
    // cube will move towards the target location
    BossCage* bossCage = blackboard.getValue<BossCage*>("bossCage");
    currentHealingCubeCount = MAX_HEALING_CUBE_COUNT;

    for (int i = 0; i < MAX_HEALING_CUBE_COUNT; i++) {
        glm::vec3 spawnPosition, cellNormal;
        spawnPosition = bossCage->getCellCenterCoords(i * (360.f / MAX_HEALING_CUBE_COUNT), 4, cellNormal);
    
        BossHealingCube* bossHealingCube = new BossHealingCube(spawnPosition, glm::vec3(0), this);
        game->add_entity(bossHealingCube);
    }

}

BT::NodeState HealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    if (currentHealingCubeCount <= 0) return BT::NodeState::FAILURE;
    if (timer <= 0) return BT::NodeState::SUCCESS; 

    timer -= deltaTime;

    return BT::NodeState::RUNNING;
}

void HealTask::onNodeFinish(BT::Blackboard& blackboard)
{
    // if state is success, call add body in boss brain
    // if state is failure, then destroy the body
}
