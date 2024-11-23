#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class BossHealingCube;

class HealCubeEntranceTask : public BT::BaseNode {
public:
	HealCubeEntranceTask(float offset);

protected:
	// Inherited via BaseNode
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	float t = 0.f;
	float timer = 0.f;

	BossHealingCube* healCube = nullptr;
};