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

protected:
	CardType cardType;

	Timer timer;

	glm::vec3 launchPosition;
	glm::vec3 launchDirection;
	glm::vec3 upDirection;

	float damage;
	float aliveTime;
	bool bDestroyOnHit;
};
