#include "SineCard.h"

#include <iostream>
#include <iomanip>

void SineCard::update(float deltaTime)
{
	timeElapsed += deltaTime * 5;

	height = glm::sin(timeElapsed) / 4.f;
	currentPosition += launchDirection * deltaTime * 3.f;

	transform.position = currentPosition + upDirection * height;

	glm::vec3 lookDirection = glm::normalize(transform.position - lastPosition);
	glm::vec3 localUp = glm::cross(lookDirection, rightDirection);
	transform.lookAt(transform.position + lookDirection, localUp);

	transform.rotation = glm::angleAxis(glm::radians(timeElapsed * 250), transform.getUp()) * transform.rotation;

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
	lastPosition = transform.position;
}

void SineCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = false;
	aliveTime = 15.0f;
	damage = 10;

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
	lastPosition = launchPosition;
	rightDirection = glm::cross(launchDirection, upDirection);
}
