#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class CosineCard : public Card
{
public:
	CosineCard(glm::vec3 position, MeshRenderer meshRenderer)
		: Card(position, meshRenderer) {}

	void update(float deltaTime) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(0.3f, 1.f, 0.3f); }

private:
	float height = 0.f;
	float timeElapsed = 0.f;
};

