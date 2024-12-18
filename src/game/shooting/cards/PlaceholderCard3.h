#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"
#include <MeshManager.h>
#include <ShaderManager.h>

class PlaceHolderCard3 : public Card
{
public:
	PlaceHolderCard3(glm::vec3 position)
		: Card(position) {
		meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
		meshRenderer->setColor(glm::vec3(0.549f, 0.322f, 1.f));
	}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(0.3f, 1.f, 0.3f); }

	CardType getCardType() const override { return Placeholder3; }

private:
	float height = 0.f;
	float timeElapsed = 0.f;

	float orbitRadius = 1.0f;
	float angle = 0.0f; 
	glm::vec3 playerPosition;  

	glm::vec3 currentPosition;
};

