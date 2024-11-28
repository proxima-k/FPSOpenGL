#include "WarningTask.h"
#include <iostream>
#include "../../Entity.h"

#include <random>

WarningTask::WarningTask(float waitTime, float scaleSpeed, bool randomScaleTimeOffset) 
	: waitTime(waitTime), scaleSpeed(scaleSpeed), randomScaleTimeOffset(randomScaleTimeOffset) {}

void WarningTask::onNodeStart(BT::Blackboard& blackboard)
{
	//std::cout << "WarningTask started +++++++++++++++++++++++++++++" << std::endl;
	timer = waitTime;

	Entity* entity = blackboard.getValue<Entity*>("entity");
	if (entity != nullptr) {
		entity->transform.scale.y = 0.01f;
		entity->transform.position.y = 0.01f;
		entity->collision_channel = Collision_Channel::None;
		
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> randomTimeOffset(0.f, 1.f/scaleSpeed);
		timeOffset = randomScaleTimeOffset ? randomTimeOffset(gen) : 0.f;
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

		float scale = 0.9f * glm::clamp(scaleSpeed * ((1 - timer / waitTime) - timeOffset), 0.f, 1.f);
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
