#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class DefaultCard : public Card
{
public:
	DefaultCard(glm::vec3 position, MeshRenderer meshRenderer)
		: Card(position, meshRenderer) {}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(1.f, 1.f, 1.f); }

private:
	float height = 0.f;
	float timeElapsed = 0.f;

	glm::vec3 currentPosition;
};

