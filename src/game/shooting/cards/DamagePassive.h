#pragma once

#include "PassiveCard.h"

class DamagePassive : public PassiveCard
{
public:
	DamagePassive(glm::vec3 position = glm::vec3(0), MeshRenderer meshRenderer = MeshRenderer())
		: PassiveCard(position, meshRenderer)
	{

	}

	~DamagePassive() override = default;

	void applyPassiveEffect() override
	{
		float currentMultiplier = game->playerDamageMultiplier;
		game->playerDamageMultiplier = currentMultiplier * 2.0f;
	}

	CardType getCardType() const override { return PassiveDamage; }
};