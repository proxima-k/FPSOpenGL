#include "WarningTask.h"
#include <iostream>
#include "../../Entity.h"

void WarningTask::onNodeStart(BT::Blackboard& blackboard)
{
	// todo: set collision channel to none

	std::cout << "WarningTask started" << std::endl;
	timer = waitTime;
}

BT::NodeState WarningTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	timer -= deltaTime;
	if (timer <= 0.0f) {
		return BT::NodeState::SUCCESS;
	}
	
	return BT::NodeState::RUNNING;
}

void WarningTask::onNodeFinish(BT::Blackboard& blackboard)
{
}
