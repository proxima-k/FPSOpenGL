#include "SineCard.h"

#include <iostream>

void SineCard::update(float deltaTime)
{
	timeElapsed += deltaTime * 5;

	height = glm::sin(timeElapsed) / 50;

	//transform.position = glm::vec3(transform.position.x, transform.position.y + height, transform.position.z);
	transform.position += launchDirection * deltaTime * 3.f + glm::vec3(0.f, height, 0.f);
	transform.rotation = glm::angleAxis(glm::radians(timeElapsed * 250), glm::vec3(0.f, 1.f, 0.f));
}
