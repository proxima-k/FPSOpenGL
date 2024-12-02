#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class BossCage;

class GrassAttackTask : public BT::BaseNode {
protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;
private:
	int MAX_WAVE = 3;
	int currentWave = 0;
	float waveTimer = 0.f;

	float warningTime = 5.f;
	float riseTime = 0.3f;
	float attackTime = 5.f;
	float fallTime = 0.5f;

	float screenShakeCountdown = warningTime;

	BossCage* cage;
};