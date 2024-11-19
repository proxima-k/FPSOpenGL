#pragma once

#include <behavior_tree/nodes/DecoratorNode.h>

#include "BossEnemy.h"
#include <cmath>

class EnoughHealthDecorator : public BT::DecoratorNode {
protected:
	bool condition(BT::Blackboard& blackboard) override { 
		BossEnemy* bossController = blackboard.getValue<BossEnemy*>("boss");

		if (currentPower <= 0) // ignore (boss is dead)
			return true;

		// if health dropped below threshold
		// if boss body count is less than pow(2, currentPower)
		if (bossController->getBodyCount() < std::pow(2, currentPower)) {
			currentPower--;
			return false;
		}

		return true; 
	}

	int currentPower = 3;
};