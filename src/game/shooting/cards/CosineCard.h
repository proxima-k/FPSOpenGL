#pragma once

#include "../Card.h"
#include <MeshManager.h>
#include <ShaderManager.h>

class CosineCard : public Card
{
public:
	CosineCard(glm::vec3 position)
		: Card(position) {
		meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);

	}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(0.3f, 1.f, 0.3f); }

	CardType getCardType() const override { return Cosine; }

private:
	float height = 0.f;
	float timeElapsed = 0.f;

	glm::vec3 currentPosition;
};

