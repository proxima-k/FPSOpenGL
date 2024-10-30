#pragma once

#include "../BaseNode.h"
#include <vector>

namespace BT {

	class CompositeNode : public BaseNode
	{
	public:
		void addChild(BaseNode* node) {
			children.push_back(node);
		}

	protected:
		std::vector<BaseNode*> children;
	};
}
