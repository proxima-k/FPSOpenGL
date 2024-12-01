#include "WaitTask.h"

#include <random>

BT::WaitTask::WaitTask(float waitTime, float variation)
	: waitTime(waitTime), variation(variation), timer(waitTime) {}

void BT::WaitTask::onNodeStart(Blackboard& blackboard)
{
	if (variation == 0) {
		timer = waitTime;
		return;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-variation, variation);
	
	timer = waitTime + dis(gen);
	if (timer < 0) {
		state = BT::NodeState::FAILURE;
		std::cout << "[Wait Node Error] Timer value less than 0" << std::endl;
	}
}


BT::NodeState BT::WaitTask::onNodeUpdate(float deltaTime, Blackboard& blackboard)
{
	if (timer <= 0)
		return BT::NodeState::SUCCESS;

	timer -= deltaTime;
	return BT::NodeState::RUNNING;
}

void BT::WaitTask::onNodeFinish(Blackboard& blackboard)
{
}