#include <iostream>
#include "Game.h"

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