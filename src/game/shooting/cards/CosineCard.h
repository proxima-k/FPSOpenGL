#pragma once

#include "../Card.h"
#include "../../../graphics/MeshRenderer.h"

class CosineCard : public Card
{
public:
	CosineCard(glm::vec3 position, MeshRenderer meshRenderer)
		: Card(position, meshRenderer) {}

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) override;

	glm::vec3 getMeshColor() const override { return glm::vec3(0.3f, 1.f, 0.3f); }

	CardType getCardType() const override { return Cosine; }

	void updateRotation(const glm::vec3& newPosition)
	{
		glm::vec3 direction = glm::normalize(newPosition - currentPosition);
		glm::vec3 right = glm::normalize(glm::cross(upDirection, direction));
		glm::vec3 up = glm::cross(direction, right);

		glm::mat3 rotationMatrix;
		rotationMatrix[0] = right;
		rotationMatrix[1] = up;
		rotationMatrix[2] = direction;

		glm::quat targetRotation = glm::quat_cast(rotationMatrix);

		transform.rotation = targetRotation;
	}

private:
	float height = 0.f;
	float timeElapsed = 0.f;

	glm::vec3 currentPosition;
};

