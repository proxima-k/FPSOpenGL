#pragma once

#include "Blackboard.h"
#include "nodes/BaseNode.h"

namespace BT
{
	class BehaviorTree {
		// reference to entity
		// stores a blackboard

	public:
		BehaviorTree() : rootNode(nullptr) {  }
		BehaviorTree(BaseNode* rootNode) : rootNode(rootNode) {  }
		~BehaviorTree();

		void update(float deltaTime) { rootNode->update(deltaTime, blackboard); }
		Blackboard& getBlackboard() { return blackboard; }
		void setRootNode(BaseNode* rootNode) { this->rootNode = rootNode; }

	private:
		BaseNode* rootNode;
		Blackboard blackboard;
	};
}