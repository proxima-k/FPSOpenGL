#pragma once

namespace SM {
	class BaseState {
	public:
		virtual void onEnterState() = 0;
		virtual void onUpdateState(float deltaTime) = 0;
		virtual void onExitState() = 0;
	};
}