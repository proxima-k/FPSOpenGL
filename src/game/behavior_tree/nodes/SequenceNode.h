#pragma once
#include "CompositeNode.h"

namespace BT {
	class SequenceNode : public CompositeNode {
	protected:
		void onNodeStart() override {
			currentChild = 0;
		}

		void onNodeExit() override {
			currentChild = -1;
		}

		NodeState onNodeUpdate(float deltaTime) override {
			while (currentChild < children.size()) {
				NodeState childState = children[currentChild]->update(deltaTime);

				if (childState == NodeState::RUNNING) {
					return NodeState::RUNNING;
				}
				else if (childState == NodeState::FAILURE) {
					return NodeState::FAILURE;
				}

				currentChild++;
			}

			return NodeState::SUCCESS;
		}
	
	private:
		int currentChild = -1;
	};
}