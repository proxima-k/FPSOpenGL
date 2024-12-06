#include "PlaceHolderCard1.h"
#include "DefaultCard.h"
#include "../Shooter.h"
#include "../../Player.h"

void PlaceHolderCard1::update(float deltaTime)
{
	glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

	transform.position += launchDirection * deltaTime * 10.f;

	glm::quat yawRotate = glm::angleAxis(glm::radians(deltaTime * 900.f), -upDirection);
	transform.rotation = yawRotate * transform.rotation;

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
	damage = 10.f;
	aliveTime = 10.f;

	if (bCanSpawnChildren)
	{
		float spreadAngle = glm::radians(3.0f);

		glm::vec3 leftDirection = glm::vec3(
			launchDirection.x * glm::cos(spreadAngle) - launchDirection.z * glm::sin(spreadAngle),
			launchDirection.y,
			launchDirection.x * glm::sin(spreadAngle) + launchDirection.z * glm::cos(spreadAngle)
		);

		glm::vec3 rightDirection = glm::vec3(
			launchDirection.x * glm::cos(-spreadAngle) - launchDirection.z * glm::sin(-spreadAngle),
			launchDirection.y,
			launchDirection.x * glm::sin(-spreadAngle) + launchDirection.z * glm::cos(-spreadAngle)
		);

		Card* leftCard = game->player->shooter.spawnCard(Card::CardType::Normal, transform.position, leftDirection, upDirection);
		Card* rightCard = game->player->shooter.spawnCard(Card::CardType::Normal, transform.position, rightDirection, upDirection);

		leftCard->meshRenderer->setColor(glm::vec3(0.3216f, 0.5216f, 1.f));
		rightCard->meshRenderer->setColor(glm::vec3(0.3216f, 0.5216f, 1.f));

		leftCard->damage = damage;
		rightCard->damage = damage;
	}

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	transform.position = launchPosition;
	transform.lookAt(transform.position + launchDirection, upDirection);
}
