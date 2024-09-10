#pragma once

#include "../Entity.h"

class Card : public Entity
{
public:
	enum CardType
	{
		Normal,
		Cosine,
		Sine,
	};

	Card(glm::vec3 position, MeshRenderer meshRenderere);

	void virtual launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);
	void update(float deltaTime) override;
	
	virtual glm::vec3 getMeshColor() const { return glm::vec3(0.5f); }

	virtual CardType getCardType() const = 0;

protected:
	CardType cardType;

	glm::vec3 launchPosition;
	glm::vec3 launchDirection;
	glm::vec3 upDirection;
};
