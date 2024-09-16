#pragma once

#include "PassiveCard.h"

class SpeedPassive : public PassiveCard
{
public:
	SpeedPassive(glm::vec3 position = glm::vec3(0), MeshRenderer meshRenderer = MeshRenderer())
		: PassiveCard(position, meshRenderer)
	{

	}

	~SpeedPassive() override = default;

	void applyPassiveEffect() override
	{
		float currentMultiplier = game->playerSpeedMultiplier;
		game->playerSpeedMultiplier = currentMultiplier * 2.0f;
	}

	CardType getCardType() const override { return PassiveSpeed; }
};