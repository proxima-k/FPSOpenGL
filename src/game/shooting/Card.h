#pragma once

#include "../Entity.h"
#include "../Enemy.h"
#include "../Game.h"

#include "../../engine/Timer.h"

class Card : public Entity
{
public:
	enum CardType
	{
		Normal,
		Cosine,
		Sine,
		Placeholder1,
		Placeholder2,
		Placeholder3,
		PassiveDamage,
		PassiveDash,
		PassiveSpeed
	};

	Card(glm::vec3 position, MeshRenderer meshRenderere);

	void virtual launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);
	void initializeTimer(float time);
	void update(float deltaTime) override;
	
	virtual glm::vec3 getMeshColor() const { return glm::vec3(0.5f); }

	virtual CardType getCardType() const = 0;

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

protected:
	CardType cardType;

	Timer timer;

	glm::vec3 launchPosition;
	glm::vec3 launchDirection;
	glm::vec3 upDirection;

	glm::vec3 currentPosition;
	glm::vec3 previousPosition;

	float damage;
	float aliveTime;
	bool bDestroyOnHit;
};
