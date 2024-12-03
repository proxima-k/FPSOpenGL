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

        changeStateInternal(mainMenuState);
    }
    ~GameStateManager() {
        delete mainMenuState;
        delete playingState;
        delete selectCardState;
        delete bossFightState;
    }

    void changeState(State targetState) {
        if (currentState == targetState) return;
        previousState = currentState;
        switch (targetState) {
        case State::MainMenu:
            changeStateInternal(mainMenuState);
            break;
        case State::Playing:
            changeStateInternal(playingState);
            break;
        case State::SelectCards:
            changeStateInternal(selectCardState);
            break;
        case State::BossFight:
            changeStateInternal(bossFightState);
            break;
        case State::Dead:
            break;
            //changeStateInternal();
        default:
            //std::cout << "state not found" << std::endl;
            break;
        }
        currentState = targetState;
    }
    
    State getCurrentState() {
        return currentState;
    }

    State getPreviousState() {
        return previousState;
    }

    Game* game = nullptr;

private:
    State currentState = State::MainMenu;
    State previousState = State::MainMenu;

    MainMenuState* mainMenuState = nullptr;
    PlayingState* playingState = nullptr;
    SelectCardState* selectCardState = nullptr;
    BossFightState* bossFightState = nullptr;
};