#pragma once

#include "Blackboard.h"
#include "BaseNode.h"

namespace BT
{
	class BehaviorTree {
		// reference to entity
		// stores a blackboard
		// stores current node?
		// evaluates from root
	public:
		BehaviorTree();
		~BehaviorTree();

		void update(float deltaTime) { rootNode->update(deltaTime); }

	private:
		BaseNode* rootNode;
		Blackboard blackboard;
	};
}