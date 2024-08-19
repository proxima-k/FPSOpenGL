#pragma once

#include "../Entity.h"

class Card : public Entity
{
public:
	Card(glm::vec3 position, MeshRenderer meshRenderer);

	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection);
	void update(float deltaTime) override;

private:
	glm::vec3 launchPosition;
	glm::vec3 launchDirection;
};