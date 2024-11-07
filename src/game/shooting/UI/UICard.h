#pragma once
#include "../Card.h"

class UICard : public Card
{
public:
	UICard(glm::vec3 position);

	void update(float deltaTime) override;

private:
	float timeElapsed = 0.f;
};

