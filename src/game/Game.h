#pragma once
#include "Entity.h"
#include "Transform.h"
#include "../engine/Timer.h"
#include "../engine/TextureManager.h"
#include "xp/XP.h"

#include <iostream>

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

    Game()
    {
        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            entitys[i] = nullptr;  // initialize all pointers to nullptr
        }

        textureManager = new TextureManager;
        textureManager->init();
    }
    ~Game()
    {
        crtPlayerXP = 0;

        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            delete entitys[i];
            entitys[i] = nullptr;
        }

        delete textureManager;
    }

    void update();
    void render();

    template<typename EntityType>
    EntityType* spawn_entity(glm::vec3 position, MeshRenderer meshRenderer)
    {
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

    Entity* get_coliding_entity(Entity* other, Collision_Channel channel);

    void spawnXP(glm::vec3 position, int xpamount)
    {
        MeshRenderer xpMesh(cubeEnemyMesh, cubeEnemyShader, camera);

        for (int i = 0; i < xpamount; i++)
        {
            spawn_entity<XP>(position, xpMesh);
        }
    }

    void addPlayerXP(int xp) {
        crtPlayerXP += xp;
        if (crtPlayerXP >= maxPlayerXP) {
            // clear all enemies
			for (int i = 0; i < MAX_ENTITYS; i++) {
				if (entitys[i] != nullptr) {
					if (entitys[i]->collision_channel == Collision_Channel::Enemy) {
						entitys[i]->destroyed = true;
					}
				}
			}
			crtPlayerXP = 0;
			scaleMaxPlayerXP();
            
            // start select card state
			currentGameState = GameStates::SelectCards;
        }
    }

    void scaleMaxPlayerXP() {
        // NOTE TO DESIGNER
        // implement your own scaling function here as you see fit

		maxPlayerXP += 100;
    }
    
    void GameOver();

    void setMeshRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);

    float deltaTime = 0.f;
    float lastFrameTime = 0.f;

    Mesh* cubeEnemyMesh;
    Shader* cubeEnemyShader;
    Camera* camera;

    TextureManager* textureManager;

    GameStates currentGameState = GameStates::Playing;

    int crtPlayerXP = 0;
    int maxPlayerXP = 100;

    float playerDamageMultiplier = 1.0f;
    float playerSpeedMultiplier = 1.0f;
    float playerDashMultiplier = 1.0f;

    bool gameOver = false;

private:
	Entity* entitys[MAX_ENTITYS] = { nullptr };

    Timer timer;

	//bool spawnEnemy = false;
};

extern Game* game;