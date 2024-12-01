#include "WaveRiseAndFallTask.h"
#include "../../Entity.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

void WaveRiseAndFallTask::onNodeStart(BT::Blackboard& blackboard)
{
	shortestHeight = 0.01f;
	Entity* entity = blackboard.getValue<Entity*>("entity");
	if (entity != nullptr) {
		entity->transform.scale.y = shortestHeight / 2;
		entity->transform.position.y = shortestHeight / 2;
		entity->collision_channel = Collision_Channel::Enemy;
		entity->meshRenderer->setColor(glm::vec3(0.3f, 0.3f, 1.f));
	}
}

BT::NodeState WaveRiseAndFallTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	if (x > glm::pi<float>()) return BT::NodeState::SUCCESS;

	Entity* entity = blackboard.getValue<Entity*>("entity");
	if (entity != nullptr) {
		float currentHeight = glm::sin(x) * MAX_HEIGHT + shortestHeight;
		entity->transform.scale.y = currentHeight;
		entity->transform.position.y = currentHeight / 2;
	}

	x += deltaTime * speed;
	
	return BT::NodeState::RUNNING;
}

void WaveRiseAndFallTask::onNodeFinish(BT::Blackboard& blackboard)
{
	Entity* entity = blackboard.getValue<Entity*>("entity");

	if (entity != nullptr) {
		entity->destroy();
	}
}
