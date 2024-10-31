#pragma once

namespace BT {
	enum class NodeState
	{
		READY,
		RUNNING,
		SUCCESS,
		FAILURE,
	};

	class BaseNode {
	public:
		// constructor and destructor
		BaseNode() = default;
		virtual ~BaseNode() = default;

		NodeState update(float deltaTime);
		NodeState finishExecuteNode(bool success);

		// events to be used for custom nodes, 
		virtual void onNodeStart() = 0;
		virtual NodeState onNodeUpdate(float deltaTime) = 0;
		virtual void onNodeFinish() = 0;
		//virtual void onNodeAborted() {}

	protected:
		NodeState state = NodeState::READY;
		//bool hasStarted = false;
	};
}