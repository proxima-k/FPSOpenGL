#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

#include <MeshManager.h>
#include <ShaderManager.h>

class SineCard : public Card
{
public:
	SineCard(glm::vec3 position)
		: Card(position) {
		meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
		meshRenderer->setColor(glm::vec3(0.494f, 0.851f, 0.341f));
	}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(1.f, 0.3f, 0.3f); }

	CardType getCardType() const override { return Sine; }

private:
	float height = 0.f;
	float timeElapsed = 0.f;
	glm::vec3 currentPosition;
	glm::vec3 lastPosition;
	glm::vec3 rightDirection;
};

