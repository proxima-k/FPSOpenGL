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
	damage = 50;
	aliveTime = 200.0f;

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

		MeshRenderer newMeshRenderer(cardMesh, cardShader, camera, glm::vec3(0.3f, 1.f, 0.3f));

		DefaultCard* leftCard = new DefaultCard(launchPosition, newMeshRenderer);
		DefaultCard* rightCard = new DefaultCard(launchPosition, newMeshRenderer);;

		game->player->shooter.spawnCard(leftCard, transform.position, leftDirection, upDirection);
		game->player->shooter.spawnCard(rightCard, transform.position, rightDirection, upDirection);

		leftCard->damage = damage;
		rightCard->damage = damage;
	}

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
	transform.position = launchPosition;

	//Initial passives
	transform.scale *= game->playerCardSizeMultiplier;
}
