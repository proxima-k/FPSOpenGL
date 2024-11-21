#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class WarningTask : public BT::BaseNode {
public:
	WarningTask(float waitTime = 1.0f);

protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	float timer = .0f;
	float waitTime = 1.0f;
};