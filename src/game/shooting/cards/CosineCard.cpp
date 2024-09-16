#include "CosineCard.h"

void CosineCard::update(float deltaTime)
{
	timeElapsed += deltaTime * 5;
	glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

	height = glm::sin(timeElapsed) / 6;
	currentPosition += launchDirection * deltaTime * 3.f;

	transform.position = currentPosition + rightVector * height;
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
}

void CosineCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = false;
	damage = 10;

	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
