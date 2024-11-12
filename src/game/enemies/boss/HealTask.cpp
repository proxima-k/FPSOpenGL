#include "HealTask.h"

void HealTask::onNodeStart(BT::Blackboard& blackboard)
{
}

BT::NodeState HealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    // get random cell on the wall grid, spawn a cube
    // assign cube's target location
    // form a line towards the target location
    // cube will move towards the target location

    return BT::NodeState::RUNNING;
}

void HealTask::onNodeFinish(BT::Blackboard& blackboard)
{
}
