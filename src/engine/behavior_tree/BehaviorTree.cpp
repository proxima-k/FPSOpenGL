#include "BehaviorTree.h"

BT::BehaviorTree::~BehaviorTree()
{
	if (rootNode != nullptr)
		delete rootNode;
}
