#pragma once

#include "../Entity.h"

class Card : public Entity
{
public:
	Card(glm::vec3 position, MeshRenderer meshRenderer);

	void virtual launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);
	void update(float deltaTime) override;
	
	virtual glm::vec3 getMeshColor() const { return glm::vec3(0.5f); }

protected:
	glm::vec3 launchPosition;
	glm::vec3 launchDirection;
	glm::vec3 upDirection;
};
