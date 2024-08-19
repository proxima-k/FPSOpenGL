#pragma once
#include "Entity.h"
#include "Transform.h"

#include <iostream>

#define MAX_ENTITYS 1000

class Game
{
public:
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

    Game()
    {
        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            entitys[i] = nullptr;  // Initialize all pointers to nullptr
        }
    }

    ~Game()
    {
        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            delete entitys[i];
            entitys[i] = nullptr;
        }
    }

    Entity* get_coliding_entity(Entity* other, Collision_Channel channel);

    float deltaTime = 0.f;
    float lastFrameTime = 0.f;

private:
	Entity* entitys[MAX_ENTITYS] = { nullptr };
};

extern Game* game;