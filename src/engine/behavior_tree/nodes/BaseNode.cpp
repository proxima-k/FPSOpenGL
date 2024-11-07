#include "BaseNode.h"

BT::NodeState BT::BaseNode::update(float deltaTime, Blackboard& blackboard) {
	// current state that represents the current frame
	// since the nodes are supposed to reset to RUNNING
	NodeState currentFrameState = NodeState::RUNNING;

	if (state == NodeState::READY) {
		state = NodeState::RUNNING;
		onNodeStart(blackboard);
	}

	// check state before update
	// if the state is still running, then perform update
	// otherwise, exit node
	if (state != NodeState::RUNNING) {
		onNodeFinish(blackboard);
		currentFrameState = state;
		state = NodeState::READY;

		return currentFrameState;
	}

	state = onNodeUpdate(deltaTime, blackboard);

	if (state != NodeState::RUNNING) {
		onNodeFinish(blackboard);
		currentFrameState = state;
		state = NodeState::READY;
	}

	return currentFrameState;
}

BT::NodeState BT::BaseNode::finishExecuteNode(bool success)
{
	state = success ? BT::NodeState::SUCCESS : BT::NodeState::FAILURE;
	return state;
}