#pragma once
#include "CompositeNode.h"
#include <iostream>

namespace BT {
	class SequenceNode : public CompositeNode {
	protected:
		void onNodeStart(Blackboard& blackboard) override {
			currentChild = 0;
		}

		void onNodeFinish(Blackboard& blackboard) override {
			currentChild = -1;
		}

		NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard) override {
			while (currentChild < children.size()) {
				NodeState childState = children[currentChild]->update(deltaTime, blackboard);

				if (childState == NodeState::RUNNING || childState == NodeState::FAILURE)
					return childState;

				currentChild++;
			}

			return NodeState::SUCCESS;
		}
	
	private:
		int currentChild = -1;
	};
}