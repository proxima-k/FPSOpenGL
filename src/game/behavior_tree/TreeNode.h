#pragma once
#include <vector>

namespace BehaviorTree
{
	enum class NodeState
	{
		RUNNING,
		SUCCESS,
		FAILURE,
	};

	class TreeNode {
	public:
		// constructor and destructor
		TreeNode() = default;
		TreeNode(TreeNode* parent) : parent(parent) {}
		TreeNode(std::vector<TreeNode*> children) : children(children) {
			for (TreeNode* child : children) 
				child->AttachSelfToParent(this);
		}
		virtual ~TreeNode() = default;

		// for handling operations every frame
		virtual void OnNodeEnter() {}
		virtual void OnNodeExit() {}
		virtual 
		virtual NodeState Evaluate(float deltaTime) { return state; }

	protected:
		TreeNode* parent = nullptr;
		std::vector<TreeNode*> children;
		NodeState state = NodeState::RUNNING;

	private:
		void AttachSelfToParent(TreeNode* parent) {
			this->parent = parent;
			parent->children.push_back(this);
		}
	};

}

