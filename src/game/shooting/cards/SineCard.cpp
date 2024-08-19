#include "SineCard.h"

#include <iostream>

void SineCard::update(float deltaTime)
{
	timeElapsed += deltaTime * 2;

	height = glm::sin(timeElapsed) / 25;

	std::cout << height << std::endl;

	//transform.position = glm::vec3(transform.position.x, transform.position.y + height, transform.position.z);
	transform.position += launchDirection * deltaTime + glm::vec3(0.f, height, 0.f);
}
