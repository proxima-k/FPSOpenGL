#pragma once

#include <state_machine/BaseState.h>
#include "../Transform.h"

class GameStateManager;
class Player;
class Camera;

class MainMenuState : public SM::BaseState {
public:
	MainMenuState(GameStateManager* gameStateManager);

	void onEnterState() override;
	void onUpdateState(float deltaTime) override;
	void onExitState() override;

private:
	// reference to the statemanager to access player
	GameStateManager* gameStateManager;
	Player* player;
	Camera* camera;
	Transform cameraSlotTf;

	float timeElapsed = 0.f;
	float t = 0.f;
};