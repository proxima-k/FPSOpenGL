#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class HealTask : public BT::BaseNode {
	// Inherited via BaseNode
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;
};