#pragma once

#include "../Entity.h"
#include "../../graphics/MeshRenderer.h"
#include "../Physicsbody.h"


class XP : public Entity
{
public:
	XP(glm::vec3 position, MeshRenderer meshRenderer);

	void update(float deltaTime) override;
};