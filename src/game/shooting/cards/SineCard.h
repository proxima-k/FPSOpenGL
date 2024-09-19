#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class SineCard : public Card
{
public:
	SineCard(glm::vec3 position, MeshRenderer meshRenderer)
		: Card(position, meshRenderer) {}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(1.f, 0.3f, 0.3f); }

	CardType getCardType() const override { return Sine; }

private:
	float timeElapsed = 0.f;

	float amplitude = 20.0f;
	float frequency = 5.0f;
};

