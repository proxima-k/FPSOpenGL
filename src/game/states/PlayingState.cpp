#include "PlayingState.h"

#include "GameStateManager.h"
#include "../Player.h"

PlayingState::PlayingState(GameStateManager* gameStateManager)
{
}

void PlayingState::onEnterState()
{
}

void PlayingState::onUpdateState(float deltaTime)
{
	// update player 
	player->update(deltaTime);

	//timeLeftUntilBoss -= deltaTime;
	//waveController->update(deltaTime);
}

void PlayingState::onExitState()
{
}
