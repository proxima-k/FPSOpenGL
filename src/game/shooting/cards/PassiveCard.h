#pragma once

#include "../Card.h"

class PassiveCard : public Card
{
public:
	PassiveCard(glm::vec3 position = glm::vec3(0), MeshRenderer meshRenderer = MeshRenderer())
		: Card(position, meshRenderer)
	{

	}
	virtual void applyPassiveEffect() 
	{		
		// passive effect base
	}
};