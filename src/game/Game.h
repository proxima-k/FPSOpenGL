#pragma once
#include "Entity.h"
#include "Transform.h"
#include "../engine/Timer.h"

#include <iostream>

#define MAX_ENTITYS 1000

class Game
{
public:
	void update();
	void render();

    // spawn an entity of type "EntityType" at the given position with the given meshRenderer
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

    Game()
    {
        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            entitys[i] = nullptr;  // initialize all pointers to nullptr
        }
    }

    // destructor to delete all entities when the game is destroyed to prevent memory leaks
    ~Game()
    {
        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            delete entitys[i];
            entitys[i] = nullptr;
        }
    }


    Entity* get_coliding_entity(Entity* other, Collision_Channel channel);
    void setMeshRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);
    void timer_callback();

    float deltaTime = 0.f;
    float lastFrameTime = 0.f;

    Mesh* cubeEnemyMesh;
    Shader* cubeEnemyShader;
    Camera* camera;

private:
	Entity* entitys[MAX_ENTITYS] = { nullptr };

    Timer timer;
};

extern Game* game;