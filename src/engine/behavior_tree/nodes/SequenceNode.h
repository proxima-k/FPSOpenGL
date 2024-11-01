#pragma once
#include "CompositeNode.h"

namespace BT {
	class SequenceNode : public CompositeNode {
	protected:
		void onNodeStart() override {
			currentChild = 0;
		}

		void onNodeFinish() override {
			currentChild = -1;
		}

		NodeState onNodeUpdate(float deltaTime) override {
			while (currentChild < children.size()) {
				NodeState childState = children[currentChild]->update(deltaTime);

				if (childState == NodeState::RUNNING || childState != NodeState::FAILURE)
					return childState;

				currentChild++;
			}

			return NodeState::SUCCESS;
		}
	
	private:
		int currentChild = -1;
	};
}