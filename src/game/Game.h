#pragma once
#include "Entity.h"
#include "Transform.h"

#define MAX_ENTITYS 10

class Game
{
public:
	void update();
	void render();

    void spawn_entity(const Entity& entity)
    {
        for (int i = 0; i < MAX_ENTITYS; ++i)
        {
            if (entitys[i] == nullptr)
            {
                entitys[i] = new Entity(entity);
                return;
            }
        }
        std::cerr << "Entity array is full, cannot spawn more entities." << std::endl;
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

private:
	Entity* entitys[MAX_ENTITYS] = { nullptr };
};

extern Game* game;