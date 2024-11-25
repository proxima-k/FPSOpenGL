#pragma once

#include <state_machine/StateManager.h>

#include "MainMenuState.h"
#include "PlayingState.h"
#include "SelectCardState.h"
#include "BossFightState.h"

class Game;

class GameStateManager : public SM::StateManager {
public:
    enum State {
        MainMenu,
        Playing,
        SelectCards,
        BossFight,
        Dead,
    };

    GameStateManager(Game* game)
        : game(game)
    {
        // initialize states here
        mainMenuState = new MainMenuState(this);
        playingState = new PlayingState(this);
        selectCardState = new SelectCardState(this);
        bossFightState = new BossFightState(this);
    }

    void changeState(State targetState) {
        switch (targetState) {
        case MainMenu:
            changeStateInternal(mainMenuState);
            break;
        case Playing:
            changeStateInternal(playingState);
            break;
        case SelectCards:
            changeStateInternal(selectCardState);
            break;
        case BossFight:
            changeStateInternal(bossFightState);
            break;
        case Dead:
            break;
            //changeStateInternal();
        default:
            //std::cout << "state not found" << std::endl;
            break;
        }
        targetState = currentState;
    }
    
    State getCurrentState() {
        return currentState;
    }

    Game* game = nullptr;

private:
    State currentState = State::MainMenu;

    MainMenuState* mainMenuState = nullptr;
    PlayingState* playingState = nullptr;
    SelectCardState* selectCardState = nullptr;
    BossFightState* bossFightState = nullptr;
};