#include "PlaceHolderCard2.h"

void PlaceHolderCard2::update(float deltaTime)
{

	glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

	currentPosition += launchDirection * deltaTime * speed;

	transform.position = currentPosition + rightVector * height;

	speed -= deltaTime * 20.0f;

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

void PlaceHolderCard2::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = false;
	damage = 20;
	speed = 10.0f;
	aliveTime = 15.f;
	transform.scale = glm::vec3(0.1f, 0.005f, 0.1f);

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
