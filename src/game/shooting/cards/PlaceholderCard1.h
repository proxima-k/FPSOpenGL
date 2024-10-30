#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class PlaceHolderCard1 : public Card
{
public:
	PlaceHolderCard1(glm::vec3 position, MeshRenderer meshRenderer)
		: Card(position, meshRenderer), bCanSpawnChildren(true) {}

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
	glm::vec3 currentPosition;

	bool bCanSpawnChildren;


};

