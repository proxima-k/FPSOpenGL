#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class WaveRiseAndFallTask : public BT::BaseNode {
public:
	WaveRiseAndFallTask(float maxHeight = 1.5f, float speed = 4) 
		: MAX_HEIGHT(maxHeight), speed(speed) {}

protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	float x = 0.f;
	float speed = 1;
	const float MAX_HEIGHT = 1.5f;

	float shortestHeight = 0.01f;
};