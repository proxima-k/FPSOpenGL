#pragma once

namespace BT {
	enum class NodeState
	{
		RUNNING,
		SUCCESS,
		FAILURE,
	};

	class BaseNode {
	public:
		// constructor and destructor
		BaseNode() = default;
		virtual ~BaseNode() = default;

		// events
		virtual void onNodeStart() = 0;
		virtual NodeState onNodeUpdate(float deltaTime) = 0;
		virtual void onNodeExit() = 0;
		//virtual void onNodeAborted() {}

		NodeState update(float deltaTime) {
			if (!hasStarted) {
				onNodeStart();
				hasStarted = true;
			}

			state = onNodeUpdate(deltaTime);

			if (state != NodeState::RUNNING) {
				onNodeExit();
				hasStarted = false;
			}

			return state;
		}

	protected:
		NodeState state = NodeState::RUNNING;
		bool hasStarted = false;
	};

}