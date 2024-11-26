#pragma once

#include <state_machine/BaseState.h>
#include "../Transform.h"
#include "../Camera.h"

class GameStateManager;
class Player;
class Game;

class PlayingState : public SM::BaseState {
public:
	PlayingState(GameStateManager* gameStateManager);

	void onEnterState() override;
	void onUpdateState(float deltaTime) override;
	void onExitState() override;

private:
	// reference to the statemanager to access player
	GameStateManager* gameStateManager = nullptr;
	Player* player = nullptr;
	Game* game = nullptr;
	Camera* camera = nullptr;

	Transform startPointTf;
	float t = 0.f;
};