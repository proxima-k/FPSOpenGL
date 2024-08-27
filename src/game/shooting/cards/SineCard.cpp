#include "SineCard.h"

#include <iostream>
#include <iomanip>

void SineCard::update(float deltaTime)
{
	timeElapsed += deltaTime * 5;

	height = glm::sin(timeElapsed) / 6;
	currentPosition += launchDirection * deltaTime * 3.f;

	transform.position = currentPosition + upDirection * height;
	transform.rotation = glm::angleAxis(glm::radians(timeElapsed * 250), -transform.getUp());
}

void SineCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;
}
