#pragma once
#include "Entity.h"
#include "Transform.h"

#define MAX_ENTITYS 10

class Game
{
public:
	void update();
	void render();

	void spawn_entity(Entity entity) 
	{
		for (int i = 0; i < MAX_ENTITYS; ++i) 
		{
			if (entitys[i] == nullptr) 
			{
				Entity new_Entity = entity;
				break;
			}
		}
	}

private:
	Entity* entitys[MAX_ENTITYS] = { nullptr };
};