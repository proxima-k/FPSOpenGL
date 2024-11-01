#pragma once

#include "BaseNode.h"

namespace BT {
	class RootNode : public BaseNode {
	public:
		~RootNode() override {
			if (child != nullptr) {
				delete child;
			}
		}

		void setChild(BaseNode* node) {
			child = node;
		}

	protected:
		void onNodeStart(Blackboard& blackboard) override {}
		void onNodeFinish(Blackboard& blackboard) override {}

		NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard) override {
			if (child != nullptr) {
				return child->update(deltaTime, blackboard);
			}

			return NodeState::SUCCESS;
		}


		BaseNode* child;
	};
}