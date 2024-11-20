#pragma once

#include "BaseNode.h"

namespace BT {

	class DecoratorNode : public BaseNode
	{
	public:
		~DecoratorNode() override {
			if (child != nullptr)
				delete child;
		}

		void setChild(BaseNode* node) {
			child = node;
		}

	protected:
		BaseNode* child = nullptr;

		virtual bool condition(Blackboard& blackboard) { return true; }

		void onNodeStart(Blackboard& blackboard) override {}
		void onNodeFinish(Blackboard& blackboard) override {}
		NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard) override {
			if (child == nullptr) return NodeState::SUCCESS;

			if (condition(blackboard)) {
				return child->update(deltaTime, blackboard);
			}
			else {
				child->abort(blackboard);
				return NodeState::FAILURE;
			}
		}

		void onNodeAbort(Blackboard& blackboard) override {
			if (child == nullptr) return;
			child->abort(blackboard);
		}
	};
}
