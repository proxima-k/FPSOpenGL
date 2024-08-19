#include "Card.h"

Card::Card(glm::vec3 position, MeshRenderer meshRenderer)
	: Entity(position, meshRenderer)
{
	
}

void Card::launch(glm::vec3 launchPosition, glm::vec3 launchDirection)
{
	this->launchPosition = launchPosition;
	this->launchDirection = launchDirection;
}

void Card::update(float deltaTime)
{
	transform.position += launchDirection * 10.f * deltaTime;
}
