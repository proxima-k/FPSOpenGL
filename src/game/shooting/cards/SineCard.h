#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class SineCard : public Card
{
public:
	SineCard(glm::vec3 position, MeshRenderer meshRenderer)
		: Card(position, meshRenderer) {}

	void update(float deltaTime) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(1.f, 0.3f, 0.3f); }

private:
	float height = 0.f;
	float timeElapsed = 0.f;
};
