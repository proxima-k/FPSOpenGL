#pragma once

#include <behavior_tree/nodes/BaseNode.h>
#include <glm/gtx/quaternion.hpp>
#include "../../Entity.h"

class BodyRotateTask : public BT::BaseNode {
protected:
	// Inherited via BaseNode
	void onNodeStart(BT::Blackboard& blackboard) override;
	BT::NodeState onNodeUpdate(float deltaTime, BT::Blackboard& blackboard) override;
	void onNodeFinish(BT::Blackboard& blackboard) override;
private:
	float t = 0;	// for lerping 
	glm::quat targetRotation;

	Entity* body;
};