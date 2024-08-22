#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class SineCard : public Card
{
public:
	SineCard(glm::vec3 position, MeshRenderer meshRenderer)
		: Card(position, meshRenderer) {}

	void update(float deltaTime) override;

private:
	float height = 0.f;
	float timeElapsed = 0.f;
};

