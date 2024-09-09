#pragma once
#include "Entity.h"
#include "Transform.h"
#include "../engine/Timer.h"
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
    }
    ~Game()
    {
        crtPlayerXP = 0;

        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            delete entitys[i];
            entitys[i] = nullptr;
        }
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

        for(int i = 0; i < xpamount; i++)
		{
			spawn_entity<XP>(position, xpMesh);
		}
    }

    void GameOver();

    void setMeshRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);

    void timer_callback();

    float deltaTime = 0.f;
    float lastFrameTime = 0.f;

    Mesh* cubeEnemyMesh;
    Shader* cubeEnemyShader;
    Camera* camera;

    GameStates currentGameState = GameStates::Playing;

    int crtPlayerXP = 0;
    int maxPlayerXP = 100;

    bool gameOver = false;

private:
	Entity* entitys[MAX_ENTITYS] = { nullptr };

    Timer timer;

	bool spawnEnemy = true;
};

extern Game* game;