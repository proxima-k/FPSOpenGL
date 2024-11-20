#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class BossHealingCube;
class BossBody;

class HealTask : public BT::BaseNode {
public:
	void notifyHealingCubeDeath(int index);

protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;

private:
	int MAX_HEALING_CUBE_COUNT = 15;
	int currentHealingCubeCount = 0;

	float MAX_HEALING_TIME = 20;
	float timer = 0;

	std::vector<BossHealingCube*> healingCubes;

	BossBody* newBossBody = nullptr;
	int bodyIndex;
};