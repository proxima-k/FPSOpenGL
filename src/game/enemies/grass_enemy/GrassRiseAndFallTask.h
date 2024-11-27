#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class Entity;

class GrassRiseAndFallTask : public BT::BaseNode {
public:
	GrassRiseAndFallTask(float riseTime, float waitTime, float fallTime, float maxHeight = 1.5f, float speed = 4)
		: riseTime(riseTime), waitTime(waitTime), fallTime(fallTime), MAX_HEIGHT(maxHeight), speed(speed) {}

protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	void updateHeight();

	Entity* entity = nullptr;
	
	float timer = 0.f;
	float speed = 1;
	float riseTime = 1.2f;
	float waitTime = 5.f;
	float fallTime = 1.2f;

	const float MAX_HEIGHT = 1.5f;
	float MIN_HEIGHT = 0.01f;
	float currentHeight = MIN_HEIGHT;

	int currentState = 0;
};