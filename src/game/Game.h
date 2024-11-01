#pragma once

#include <iostream>

#include "Entity.h"
#include "Transform.h"

#include "../engine/Timer.h"
#include "../engine/TextureManager.h"
#include "../engine/ParticleController.h"

class Player;
class Shooter;

#define MAX_ENTITYS 1000

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

    template<typename EntityType>
    EntityType* spawn_entity(glm::vec3 position, MeshRenderer meshRenderer) {
        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            if (entitys[i] == nullptr)
            {
                EntityType* new_entity = new EntityType(position, meshRenderer);
                entitys[i] = new_entity;

                return new_entity;
            }
        }
        std::cerr << "Entity array is full, cannot spawn more entities." << std::endl;
        return nullptr;
    }

    void spawn_particle_source(glm::vec3 startPos, const int particleAmount, const float duration, const float speed) {
        for (int i = 0; i < 5; i++) {
            if (particleCtrl[i] == nullptr)
            {
                particleCtrl[i] = new ParticleController(startPos, particleAmount, duration, speed);
                break;
            }
        }
    }

    Entity* get_coliding_entity(Entity* other, Collision_Channel channel);

    void setMeshRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);

    void enterSelectCardState();
    void addPlayerXP(int xp) { crtPlayerXP += xp; enterSelectCardState(); }
    void level_up_player() { maxPlayerXP += 100; } // implement scaling function here

    void gameOver();
    void reset();

    int getPlayerXP() { return crtPlayerXP; }
    int getPlayerMaxXP() { return maxPlayerXP; }
    int get_player_level() { return playerLevel; }

    Mesh* cubeEnemyMesh;
    Shader* cubeEnemyShader;
    Camera* camera;
    Player* player;

    TextureManager* textureManager;

    GameStates currentGameState = GameStates::Playing;

    float playerDamageMultiplier = 1.0f;
    float playerSpeedMultiplier = 1.0f;
    float playerDashMultiplier = 1.0f;

    bool bGameOver = false;

private:
    float deltaTime = 0.f;
    float lastFrameTime = 0.f;

    int crtPlayerXP = 0;
    int maxPlayerXP = 100;
    int playerLevel = 1;

	Entity* entitys[MAX_ENTITYS] = { nullptr };
    ParticleController* particleCtrl[20] = { nullptr };
    Timer timer;
};

extern Game* game;