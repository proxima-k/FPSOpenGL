#pragma once

// set collision channel to enemy
#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class AttackTask : public BT::BaseNode {
protected:
	void onNodeStart(BT::Blackboard& blackboard) override;

	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;

	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	float currentHeight = 0.01f;
	const float MAX_HEIGHT = 5.f;
};