#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class BossEnemy;

class BossEntranceTask : public BT::BaseNode {
protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	void setBossBodiesHealth();

	float tValue = 0.f;
	float maxTValue = 8.f;

	BossEnemy* boss;
};