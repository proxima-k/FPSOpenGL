#pragma once

#include "PassiveCard.h"

class SizePassive : public PassiveCard
{
public:
	SizePassive(glm::vec3 position = glm::vec3(0), MeshRenderer meshRenderer = MeshRenderer())
		: PassiveCard(position, meshRenderer)
	{

	}

	~SizePassive() override = default;

	void applyPassiveEffect() override
	{
		float currentMultiplier = game->playerCardSizeMultiplier;
		game->playerCardSizeMultiplier = currentMultiplier * 1.2f;
	}

	CardType getCardType() const override { return PassiveSize; }
};
