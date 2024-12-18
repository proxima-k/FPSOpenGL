#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"
#include <MeshManager.h>
#include <ShaderManager.h>

class PlaceHolderCard1 : public Card
{
public:
	PlaceHolderCard1(glm::vec3 position)
		: Card(position), bCanSpawnChildren(true) {
		meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
		meshRenderer->setColor(glm::vec3(0.3216f, 0.5216f, 1.f));
	}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(0.3f, 1.f, 0.3f); }
	CardType getCardType() const override { return Placeholder1; }

	Mesh* cardMesh;
	Shader* cardShader;
	Camera* camera;

private:
	float height = 0.f;
	float timeElapsed = 0.f;

	bool bCanSpawnChildren;
};

