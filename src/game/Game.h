#pragma once

#include <iostream>

#include "Entity.h"
#include "Transform.h"

#include "../engine/Timer.h"
#include "../engine/TextureManager.h"
#include "../engine/ParticleController.h"
#include "../game/enemies/boss/HealingLine.h"

#include "enemies/wave_system/WaveController.h"

#include "states/GameStateManager.h"
#include "BossFightController.h"

class Player;
class Shooter;
class Grid;
class YAxisLine;

#define MAX_ENTITYS 1000
#define MAX_PARTICLE_CTRLS 15
#define MAX_HEAL_LINES 30

class Game
{
public:
    enum GameStates
    {
        Playing,
        Dead,
        SelectCards,
        Menu,
    };

    Game();
    ~Game();

    void update();
    void render();
    void init() {
        bossFightController = new BossFightController();
        waveController = new WaveController();
        waveController->populate_queue();
        gameStateManager = new GameStateManager(this);
        //gameStateManager->changeState(GameStateManager::State::MainMenu);
    }

    template<typename EntityType>
    EntityType* spawn_entity(glm::vec3 position) {
        for (int i = 0; i < MAX_ENTITYS; i++)
        {
            if (entitys[i] == nullptr)
            {
                EntityType* new_entity = new EntityType(position);
                entitys[i] = new_entity;

                return new_entity;
            }
        }
        std::cerr << "Entity array is full, cannot spawn more entities." << std::endl;
        return nullptr;
    }

    template<typename EntityType>
    EntityType* add_entity(EntityType* entity) {
        if (entity == nullptr) {
            std::cerr << "Entity to add is null" << std::endl;
            return nullptr;
        }

        for (int i = 0; i < MAX_ENTITYS; i++) 
        {
            if (entitys[i] == nullptr)
            {
                entitys[i] = entity;
                return entity;
            }
        }

        std::cerr << "Entity array is full, cannot spawn more entities." << std::endl;
        return nullptr;
    }

    void spawn_particle_source(glm::vec3 startPos, const int particleAmount, const float duration, const float speed) {
        for (int i = 0; i < MAX_PARTICLE_CTRLS; i++) {
            if (particleCtrl[i] == nullptr)
            {
                particleCtrl[i] = new ParticleController(startPos, particleAmount, duration, speed);
                break;
            }
        }
    }

    void renderHealingLines();
    HealingLine* spawn_healing_line(glm::vec3 startPos, glm::vec3 endPos) {
        for (int i = 0; i < MAX_HEAL_LINES; i++) {
            if (healingLines[i] == nullptr)
            {
                healingLines[i] = new HealingLine(startPos, endPos);
                return healingLines[i];
            }
        }

        std::cerr << "healing line array is full, cannot spawn more lines." << std::endl;
        return nullptr;
    }

    void renderGrid();

    Entity* get_coliding_entity(Entity* other, Collision_Channel channel);
    Entity* get_colliding_entity_OBB(Entity* other, Collision_Channel channel);

    void enterSelectCardState();
    void addPlayerXP(int xp) { 
        if (getCurrentState() != GameStateManager::State::Playing) return;
        crtPlayerXP += xp; 
        enterSelectCardState(); 
    }
    void level_up_player() {
        playerLevel++;
        maxPlayerXP += pow(100, ((playerLevel*1.5f)+15) / 20);
        waveController->next_wave();
    } // implement scaling function here

    void gameOver();
    void reset();
    void clearEnemies();

    int getPlayerXP() { return crtPlayerXP; }
    int getPlayerMaxXP() { return maxPlayerXP; }
    int get_player_level() { return playerLevel; }

    void changeState(GameStateManager::State newState) { gameStateManager->changeState(newState); }
    GameStateManager::State getCurrentState() { return gameStateManager->getCurrentState(); }
    bool isInGame() {
        return (getCurrentState() == GameStateManager::State::Playing || getCurrentState() == GameStateManager::State::BossFight);
    }

    void startCredits();
    bool bWin = false;

    Camera* camera;
    Player* player;

    TextureManager* textureManager;
    WaveController* waveController = nullptr;
    BossFightController* bossFightController = nullptr;
    GameStateManager* gameStateManager = nullptr;
    Grid* baseFloorGrid = nullptr;
    YAxisLine* yAxisLine = nullptr;

    //GameStates currentGameState = GameStates::Menu;

    float playerDamageMultiplier = 1.0f;
    float playerSpeedMultiplier = 1.0f;
    float playerDashMultiplier = 1.0f;

    bool bGameOver = false;

    int minutesUntilBossSpawns = 5;
    float timeLeftUntilBoss = (minutesUntilBossSpawns * 60 + 1);
    //float timeLeftUntilBoss = 10.f;

private:
    float deltaTime = 0.f;
    float lastFrameTime = 0.f;

    int crtPlayerXP = 0;
    int maxPlayerXP = 100;
    int playerLevel = 1;

	Entity* entitys[MAX_ENTITYS] = { nullptr };
    ParticleController* particleCtrl[20] = { nullptr };
    HealingLine* healingLines[MAX_HEAL_LINES] = { nullptr };
    Timer timer;
};

extern Game* game;