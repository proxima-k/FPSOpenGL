#include "AttackTask.h"
#include <iostream>
#include "../../Entity.h"

void AttackTask::onNodeStart(BT::Blackboard& blackboard)
{
	currentHeight = 0.01f;
	std::cout << "AttackTask started" << std::endl;
}

BT::NodeState AttackTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	Entity* entity = blackboard.getValue<Entity*>("entity");


	currentHeight += deltaTime * 2.0f;
	entity->transform.scale.y = currentHeight;
	entity->transform.position.y = currentHeight;

	if (currentHeight >= MAX_HEIGHT) {
		return BT::NodeState::SUCCESS;
	}
	return BT::NodeState::RUNNING;
}

void AttackTask::onNodeFinish(BT::Blackboard& blackboard)
{
}


// update entity position and scale
// something like the following

/*
void PillarEnemy::update(float deltaTime)
{
	waitTimer -= deltaTime;
	if (waitTimer > 0.f)
	{
		return;
	}

	//currentHeight = glm::mix(currentHeight, 2.f, 0.03f);
	if (currentHeight >= 2.f)
	{
		//currentHeight = 0.01f;
		return;
	}
	currentHeight += 10.f * deltaTime;
	updateHeight();

	/*std::cout << "======================" << std::endl;
	std::cout << "current height: " << currentHeight << std::endl;
	std::cout << "transform scale: " << transform.scale.y << std::endl;
	std::cout << "y position: " << transform.position.y << std::endl;
}


void PillarEnemy::updateHeight()
{
	transform.scale.y = currentHeight;
	transform.position.y = currentHeight;
}
*/
