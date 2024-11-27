#include "WarningTask.h"
#include <iostream>
#include "../../Entity.h"

WarningTask::WarningTask(float waitTime, float speedOffset) 
	: waitTime(waitTime), speedOffset(speedOffset) {}

void WarningTask::onNodeStart(BT::Blackboard& blackboard)
{
	//std::cout << "WarningTask started +++++++++++++++++++++++++++++" << std::endl;
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
	

	if (timer <= 0.0f) {
		return BT::NodeState::SUCCESS;
	}
	
	timer -= deltaTime;
	
	Entity* entity = blackboard.getValue<Entity*>("entity");
	if (entity != nullptr) {
		// clamp 2 * (1 - timer/waitTime)
		
		float scale = 0.9f * glm::clamp(speedOffset * (1 - timer / waitTime), 0.f, 1.f);
		entity->transform.scale.x = scale;
		entity->transform.scale.z = scale;
	}

	return BT::NodeState::RUNNING;
}

void WarningTask::onNodeFinish(BT::Blackboard& blackboard)
{
	Entity* entity = blackboard.getValue<Entity*>("entity");
	if (entity != nullptr) {
		entity->transform.scale.x = 0.9f;
		entity->transform.scale.z = 0.9f;
	}
}
