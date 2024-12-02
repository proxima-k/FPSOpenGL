#pragma once

#include "BaseState.h"

namespace SM {
	class StateManager {
	public:
		StateManager() = default;

		void update(float deltaTime) {
			if (currentStateNode == nullptr) return;

			currentStateNode->onUpdateState(deltaTime);
		}

	protected:
		void changeStateInternal(BaseState* newState) {
			if (currentStateNode != nullptr)
				currentStateNode->onExitState();
		
			currentStateNode = newState;
			currentStateNode->onEnterState();
		}

		BaseState* currentStateNode = nullptr;
	};
}