#pragma once

#include "CompositeNode.h"
#include <random>

namespace BT {
	class RandomSelectorNode : public CompositeNode {
	protected:
		void onNodeStart(Blackboard& blackboard) override {
			if (children.size() == 1) {
				currentChildIndex = 0;
				return;
			}

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<int> randomIndex(0, children.size() - 1);
			
			for (int i = 0; i < 100; i++) {
				currentChildIndex = randomIndex(gen);
				if (currentChildIndex != lastChildIndex) break;
			}
		};

		NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard) override {
			return children[currentChildIndex]->update(deltaTime, blackboard);

			/*if (childState == NodeState::RUNNING)
				return childState;

			return NodeState::SUCCESS;*/
		}

		void onNodeFinish(Blackboard& blackboard) override {
			lastChildIndex = currentChildIndex;
		}

	private:
		int currentChildIndex;
		int lastChildIndex = -1;
	};
}