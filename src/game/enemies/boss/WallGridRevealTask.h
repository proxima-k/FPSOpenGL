#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class WallGridRevealTask : public BT::BaseNode {
protected:
	// Inherited via BaseNode
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	float t = 0.f;
	float intensity = 1.f;
};