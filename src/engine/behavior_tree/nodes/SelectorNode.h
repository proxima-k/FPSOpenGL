#pragma once

#include "CompositeNode.h"

namespace BT {
	class SelectorNode : public CompositeNode {
	protected:
		void onNodeStart() override {
			currentChildIndex = 0;
		}

		void onNodeFinish() override {
			currentChildIndex = -1;
		}

		NodeState onNodeUpdate(float deltaTime) override {
			while (currentChildIndex < children.size()) {
				NodeState childState = children[currentChildIndex]->update(deltaTime);

				if (childState == NodeState::RUNNING || childState != NodeState::SUCCESS)
					return childState;

				currentChildIndex++;
			}

			return NodeState::FAILURE;
		}

	private:
		int currentChildIndex;
	};
}