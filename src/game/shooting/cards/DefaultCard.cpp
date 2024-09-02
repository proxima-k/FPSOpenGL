#include "DefaultCard.h"
#include "../../game.h"

void DefaultCard::update(float deltaTime)
{
	glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

	currentPosition += launchDirection * deltaTime * 10.f;

	transform.position = currentPosition + rightVector * height;

	Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
	if (hit_actor != nullptr)
	{
		destroy();
	}
}

void DefaultCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
