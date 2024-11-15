#include "FloorGridRevealTask.h"

void FloorGridRevealTask::onNodeStart(BT::Blackboard& blackboard)
{
}

BT::NodeState FloorGridRevealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	return BT::NodeState::RUNNING;
}

void FloorGridRevealTask::onNodeFinish(BT::Blackboard& blackboard)
{
}
