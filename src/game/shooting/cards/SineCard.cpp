#include "SineCard.h"

#include <iostream>
#include <iomanip>

void SineCard::update(float deltaTime)
{
	timeElapsed += deltaTime * 5;

	height = glm::sin(timeElapsed) / 6;
	currentPosition += launchDirection * deltaTime * 3.f;

	transform.position = currentPosition + upDirection * height;
	transform.rotation = glm::angleAxis(glm::radians(timeElapsed * 250), -transform.getUp());

	Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
	if (hit_actor != nullptr)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(hit_actor);
		if (enemy != nullptr)
		{
			enemy->take_damage(damage * game->playerDamageMultiplier);

			if (bDestroyOnHit)
				destroy();
		}
	}

	timer.updateTimer(deltaTime);
}

void SineCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = false;
	aliveTime = 15.0f;
	damage = 10;

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
