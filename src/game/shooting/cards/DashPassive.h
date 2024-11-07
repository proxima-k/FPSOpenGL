#pragma once

#include "PassiveCard.h"

class DashPassive : public PassiveCard
{
public:
	DashPassive(glm::vec3 position = glm::vec3(0))
		: PassiveCard(position)
	{

	}

	~DashPassive() override = default;

	void applyPassiveEffect() override
	{
		float currentMultiplier = game->playerDashMultiplier;
		game->playerDashMultiplier = currentMultiplier * 2.0f;
	}

	CardType getCardType() const override { return PassiveDash; }
};