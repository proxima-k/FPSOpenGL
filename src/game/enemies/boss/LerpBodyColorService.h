#pragma once

#include <behavior_tree/nodes/ServiceNode.h>

class LerpBodyColorService : public BT::ServiceNode {
protected:
	void serviceUpdate(float deltaTime, BT::Blackboard& blackboard) {
		// get boss controller from blackboard
		// get current color
		// get target color from blackboard
		// lerp current color to target color
		// call bossController->setMeshColor(lerpedColor)

	}
};