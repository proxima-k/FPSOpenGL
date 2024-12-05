#pragma once

#include <behavior_tree/nodes/BaseNode.h>

class BossCage;

class WaveAttackTask : public BT::BaseNode {
protected:
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;
private:
	void resetValues();

	float timer = 0.f;
	float timeToSpawnWave = 0.2f;
	
	int currentXCellCount = 0;
	int currentZCellCount = 0;
	int stepsLeft = 0;

	float loopTimer = 0.f;
	float timeToSpawnLoop = 0.1f;
	int loopsLeft = 0;
	int totalLoops = 4;

	bool hasShakedCamera = true;
	float SHAKE_WAIT_TIME = 1.f;
	float shakeCountdown = 0.f;

	BossCage* cage;
};