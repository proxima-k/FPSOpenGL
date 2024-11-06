#include "WarningTask.h"
#include <iostream>
#include "../../Entity.h"

void WarningTask::onNodeStart(BT::Blackboard& blackboard)
{
	std::cout << "WarningTask started +++++++++++++++++++++++++++++" << std::endl;
	timer = waitTime;

	Entity* entity = blackboard.getValue<Entity*>("entity");
	if (entity != nullptr) {
		entity->transform.scale.y = 0.01f;
		entity->transform.position.y = 0.01f;
		entity->collision_channel = Collision_Channel::None;
	}
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
