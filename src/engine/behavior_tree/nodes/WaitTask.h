#pragma once

#include "BaseNode.h"

namespace BT {
	class WaitTask : public BaseNode {
	public:
		WaitTask(float waitTime=1, float variation=0);

	protected:
		void onNodeStart(Blackboard& blackboard) override;
		NodeState onNodeUpdate(float deltaTime, Blackboard& blackboard);
		void onNodeFinish(Blackboard& blackboard) override;
	private:
		float timer, waitTime, variation;
	};
}