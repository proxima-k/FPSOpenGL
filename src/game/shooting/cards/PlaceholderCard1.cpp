#include "PlaceHolderCard1.h"
#include "DefaultCard.h"
#include "../Shooter.h"
#include "../../Player.h"

void PlaceHolderCard1::update(float deltaTime)
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

			if (bDestroyOnHit)
				destroy();
		}
	}

	timer.updateTimer(deltaTime);
}

void PlaceHolderCard1::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = false;
	damage = 10;

	if (bCanSpawnChildren)
	{
		glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

		float spreadAngle = glm::radians(10.0f);

		/*PlaceHolderCard1* leftCard;
		game->player->shooter.spawnCard(leftCard, transform.position, launchDirection + spreadAngle, upDirection);
		leftCard->bCanSpawnChildren = false;
		leftCard->launch(launchPosition, launchDirection + spreadAngle, upDirection);

		PlaceHolderCard1* rightCard;
		game->player->shooter.spawnCard(rightCard, transform.position, launchDirection + -spreadAngle, upDirection);
		rightCard->bCanSpawnChildren = false;
		rightCard->launch(launchPosition, launchDirection + -spreadAngle, upDirection);*/

		game->player->shooter.shootDefault(transform.position, launchDirection + spreadAngle, upDirection);
		game->player->shooter.shootDefault(transform.position, launchDirection - spreadAngle, upDirection);
	}

	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
