#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"
#include <MeshManager.h>
#include <ShaderManager.h>

// pull card
class PlaceHolderCard2 : public Card
{
public:
	PlaceHolderCard2(glm::vec3 position)
		: Card(position) {
		meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
		meshRenderer->setColor(glm::vec3(1.f, 0.7411f, 0.349f));
	}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(0.3f, 1.f, 0.3f); }

	CardType getCardType() const override { return Placeholder2; }

private:
	float height = 0.f;
	float timeElapsed = 0.f;
	float elapsedTime = 0.f;
	float speed = 0.f;

	glm::vec3 currentPosition;
};

