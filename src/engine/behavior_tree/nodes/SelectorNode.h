#pragma once

#include "CompositeNode.h"

namespace BT {
	class SelectorNode : public CompositeNode {
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

				if (childState == NodeState::RUNNING || childState != NodeState::SUCCESS)
					return childState;

				currentChildIndex++;
			}

			return NodeState::FAILURE;
		}

		void onNodeAbort(Blackboard& blackboard) override {
			children[currentChildIndex]->abort(blackboard);
			currentChildIndex = -1;
			BaseNode::onNodeAbort(blackboard);
		}

	private:
		int currentChildIndex;
	};
}