#pragma once

#include "Blackboard.h"
#include "TreeNode.h"

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

		void update(float deltaTime) { rootNode->evaluate(deltaTime); }

	private:
		TreeNode* rootNode;
		Blackboard blackboard;
	};
}