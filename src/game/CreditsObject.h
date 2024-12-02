#pragma once

#include "Entity.h"

class CreditObject : public Entity {
public:
	CreditObject(glm::vec3 position);
	void update(float deltaTime) override;
};