#pragma once

#include "../Entity.h"
#include "../../graphics/MeshRenderer.h"
#include "../Physicsbody.h"
#include "../../graphics/TrailMesh.h"

class XP : public Entity
{
public:
	XP(glm::vec3 position, MeshRenderer meshRenderer);

	void update(float deltaTime) override;

private:
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};