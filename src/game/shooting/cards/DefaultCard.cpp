#include "DefaultCard.h"
#include "../../game.h"

DefaultCard::DefaultCard(glm::vec3 position, MeshRenderer meshRenderer)
	: Card(position, meshRenderer)
{
}

void DefaultCard::update(float deltaTime)
{
	glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

	currentPosition += launchDirection * deltaTime * 10.f;

	transform.position = currentPosition + rightVector * height;

	Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
	if (hit_actor != nullptr)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(hit_actor);
		if (enemy != nullptr)
		{
			enemy->take_damage(damage * game->playerDamageMultiplier);

			game->spawn_particle_source(transform.position, 40.0f, 0.4f);

			if(bDestroyOnHit)
				destroy();
		}
	}

	timer.updateTimer(deltaTime);
}

void DefaultCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = true;
	aliveTime = 200.0f;
	damage = 5;

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
