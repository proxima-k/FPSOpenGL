#include "CosineCard.h"

void CosineCard::update(float deltaTime)
{
	timeElapsed += deltaTime * 5;
	glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

	height = glm::sin(timeElapsed) / 6;
	currentPosition += launchDirection * deltaTime * 3.f;

	transform.position = currentPosition + rightVector * height;
	transform.rotation = glm::angleAxis(glm::radians(timeElapsed * 250), -transform.getUp());
}

void CosineCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
