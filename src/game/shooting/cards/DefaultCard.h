#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class DefaultCard : public Card
{
public:
	DefaultCard(glm::vec3 position);

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(1.f, 1.f, 1.f); }

	CardType getCardType() const override { return Normal; }

private:
	float timeElapsed = 0.f;

};

