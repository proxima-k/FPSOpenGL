#pragma once

#include "PassiveCard.h"

class DamagePassive : public PassiveCard
{
public:
	DamagePassive(glm::vec3 position = glm::vec3(0))
		: PassiveCard(position)
	{

	}

	~DamagePassive() = default;

	void applyPassiveEffect() override
	{
		float currentMultiplier = game->playerDamageMultiplier;
		game->playerDamageMultiplier = currentMultiplier * 1.5f;
	}

	CardType getCardType() const override { return PassiveDamage; }
};