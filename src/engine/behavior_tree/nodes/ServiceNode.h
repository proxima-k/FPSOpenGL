#pragma once

#include <behavior_tree/nodes/BaseNode.h>

namespace BT {
	class ServiceNode : public BaseNode {
	public:
		~ServiceNode() override {
			if (child != nullptr) delete child;
		}

		void setChild(BaseNode* node) { child = node; }

	protected:
		BaseNode* child = nullptr;

		virtual void serviceUpdate(float deltaTime, Blackboard& blackboard) = 0;

		void onNodeStart(Blackboard& blackboard) override {}
		void onNodeFinish(Blackboard& blackboard) override {}
		NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard) override {
			serviceUpdate(deltaTime, blackboard);

			if (child != nullptr) 
				return child->update(deltaTime, blackboard);
			
			return NodeState::SUCCESS;
		}
	};
}