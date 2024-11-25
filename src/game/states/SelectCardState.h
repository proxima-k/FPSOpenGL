#pragma once

#include <state_machine/BaseState.h>

class GameStateManager;
//class Player;

class SelectCardState : public SM::BaseState {
public:
	SelectCardState(GameStateManager* gameStateManager);

	void onEnterState() override;
	void onUpdateState(float deltaTime) override;
	void onExitState() override;

private:
	// reference to the statemanager to access player
	GameStateManager* gameStateManager;
	//Player* player;
};