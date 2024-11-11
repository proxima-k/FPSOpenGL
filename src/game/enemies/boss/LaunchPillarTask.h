#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class LaunchPillarTask : public BT::BaseNode {
protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;
private:
	float timer = 0.f;
	float attackCooldown = 0.3f;

	int maxAttackCount = 5;
	int currentAttackCount = 0;
};