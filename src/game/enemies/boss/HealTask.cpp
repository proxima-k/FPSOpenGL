#include "HealTask.h"

#include "BossCage.h"

void HealTask::onNodeStart(BT::Blackboard& blackboard)
{
}

BT::NodeState HealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    BossCage* bossCage = blackboard.getValue<BossCage*>("bossCage");

    // get random cell on the wall grid, spawn a cube
    // assign cube's target location
    // form a line towards the target location
    // cube will move towards the target location

    return BT::NodeState::RUNNING;
}

void HealTask::onNodeFinish(BT::Blackboard& blackboard)
{
}
