#pragma once

#include "../Blackboard.h"

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

		NodeState update(float deltaTime, Blackboard& blackboard);
		NodeState finishExecuteNode(bool success);

	protected:
		// events to be used for custom nodes, 
		virtual void onNodeStart(Blackboard& blackboard) = 0;
		virtual NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard) = 0;
		virtual void onNodeFinish(Blackboard& blackboard) = 0;
		//virtual void onNodeAborted() {}

		NodeState state = NodeState::READY;
		//bool hasStarted = false;
	};
}