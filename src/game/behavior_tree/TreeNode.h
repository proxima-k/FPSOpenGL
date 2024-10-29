
#pragma once
#include <vector>

namespace BT
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
				child->attachSelfToParent(this);
		}
		virtual ~TreeNode() = default;

		// events 
		virtual void onNodeEnter() {}
		virtual void onNodeExit() {}
		virtual void onNodeAborted() {}
		
		virtual NodeState evaluate(float deltaTime) { return state; }

	protected:
		TreeNode* parent = nullptr;
		std::vector<TreeNode*> children;
		NodeState state = NodeState::RUNNING;

	private:
		void attachSelfToParent(TreeNode* parent) {
			this->parent = parent;
			parent->children.push_back(this);
		}
	};

}

