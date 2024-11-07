#include "UICard.h"

UICard::UICard(glm::vec3 position)
	: Card(position)
{
}

void UICard::update(float deltaTime)
{
	timeElapsed += deltaTime;
	// if timeElapsed is too high, reset it

	if (timeElapsed > glm::pi<float>())
		timeElapsed -= glm::pi<float>();

	// float up and down
	transform.position.y += sin(timeElapsed * 2.0f) * 0.05f;
}
