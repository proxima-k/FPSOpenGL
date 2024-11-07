#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class PlaceHolderCard3 : public Card
{
public:
	PlaceHolderCard3(glm::vec3 position)
		: Card(position) {}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(0.3f, 1.f, 0.3f); }

	CardType getCardType() const override { return Placeholder3; }

private:
	float height = 0.f;
	float timeElapsed = 0.f;

	glm::vec3 currentPosition;
};

