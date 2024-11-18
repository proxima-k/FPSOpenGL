#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class HealTask : public BT::BaseNode {
public:
	void NotifyHealingCubeDeath() { currentHealingCubeCount--; };

protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	int MAX_HEALING_CUBE_COUNT = 5;
	int currentHealingCubeCount = 0;

	float MAX_HEALING_TIME = 20;
	float timer = 0;
};