#include <iostream>
#include "Game.h"
#include "Player.h"
#include "AABB.h"

void Game::update() 
{
	for (int i = 0; i < MAX_ENTITYS; i++)
	{
		if (entitys[i] != nullptr)
		{
			if (entitys[i]->destroyed)
			{
				delete entitys[i];
				entitys[i] = nullptr;
			}
			else 
			{
				entitys[i]->update();
			}
		}
	}
}

void Game::render()
{
	for (int i = 0; i < MAX_ENTITYS; i++)
	{
		if (entitys[i] != nullptr)
		{
			entitys[i]->draw();
		}
	}
}

Entity* Game::get_coliding_entity(Entity* other, Collision_Channel channel) 
{
	for (int i = 0; i < MAX_ENTITYS; i++) 
	{
		if (entitys[i] == other)
			continue;

		if (entitys[i] == nullptr)
			continue;

		if (entitys[i]->collision_channel != channel)
			continue;

		AABB a = AABB::from_position_size(other->transform);
		AABB b = AABB::from_position_size(entitys[i]->transform);

		if (aabb_overlap(a, b)) {
			return entitys[i];
		}
	}

	return nullptr;
}