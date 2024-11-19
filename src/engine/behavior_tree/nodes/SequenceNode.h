#pragma once
#include "CompositeNode.h"
#include <iostream>

namespace BT {
	class SequenceNode : public CompositeNode {
	protected:
		void onNodeStart(Blackboard& blackboard) override {
			currentChildIndex = 0;
		}

		void onNodeFinish(Blackboard& blackboard) override {
			currentChildIndex = -1;
		}

		NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard) override {
			while (currentChildIndex < children.size()) {
				NodeState childState = children[currentChildIndex]->update(deltaTime, blackboard);

				if (childState == NodeState::RUNNING || childState == NodeState::FAILURE)
					return childState;

				currentChildIndex++;
			}

			return NodeState::SUCCESS;
		}

		void onNodeAbort(Blackboard& blackboard) override {
			children[currentChildIndex]->abort(blackboard);
			currentChildIndex = -1;
			BaseNode::onNodeAbort(blackboard);
		}
	
	private:
		int currentChildIndex = -1;
	};
}