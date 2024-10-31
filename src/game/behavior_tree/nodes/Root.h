#pragma once

#include "../BaseNode.h"

namespace BT {
	class RootNode : public BaseNode {
	public:
		void setChild(BaseNode* node) {
			child = node;
		}

	protected:
		void onNodeStart() override {}
		void onNodeFinish() override {}

		NodeState onNodeUpdate(float deltaTime) override {
			if (child != nullptr) {
				return child->update(deltaTime);
			}

			return NodeState::SUCCESS;
		}


		BaseNode* child;
	};
}