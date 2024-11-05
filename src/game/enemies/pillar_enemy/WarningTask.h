#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class WarningTask : public BT::BaseNode {
protected:
	void onNodeStart(BT::Blackboard& blackboard) override;

	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;

	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	float timer = .0f;
	float waitTime = 2.0f;
};