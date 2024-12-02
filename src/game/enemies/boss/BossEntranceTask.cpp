#include "BossEntranceTask.h"

#include <glm/glm.hpp>

#include "BossEnemy.h"
#include "BossBody.h"

void BossEntranceTask::onNodeStart(BT::Blackboard& blackboard)
{
	boss = blackboard.getValue<BossEnemy*>("boss");
	if (boss == nullptr) {
		state = BT::NodeState::FAILURE;
		return;
	}
	for (BossBody* body : boss->bossBodies) {
		body->smoothSizing = true;
	}

	boss->setCanCollide(false);
}

BT::NodeState BossEntranceTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	if (tValue >= maxTValue/1.2f - 0.001) return BT::NodeState::SUCCESS;
	

	setBossBodiesHealth();
	tValue = glm::mix(tValue, maxTValue, 0.015f);

	return BT::NodeState::RUNNING;
}

void BossEntranceTask::onNodeFinish(BT::Blackboard& blackboard)
{
	for (BossBody* body : boss->bossBodies) {
		body->health = body->maxHealth;
		body->smoothSizing = false;
	}
	boss->setCanCollide(true);
}

void BossEntranceTask::setBossBodiesHealth()
{
	for (BossBody* body : boss->bossBodies) {
		// set health based on current tValue
		int currentIndex = body->index;
		body->health = glm::clamp((tValue - currentIndex/2.f), 0.f, 1.f) * body->maxHealth;
	}
}
