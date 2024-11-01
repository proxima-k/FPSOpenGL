#pragma once

#include "BaseNode.h"
#include <vector>

namespace BT {

	class CompositeNode : public BaseNode
	{
	public:
		~CompositeNode() override {
			for (auto child : children) {
				if (child != nullptr)
					delete child;
			}
		}

		void addChild(BaseNode* node) {
			children.push_back(node);
		}

	protected:
		std::vector<BaseNode*> children;
	};
}
