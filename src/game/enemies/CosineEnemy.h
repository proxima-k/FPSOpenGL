#pragma once

#include "../Enemy.h"
#include "../Physicsbody.h"
#include "../Player.h"
#include "../../graphics/TrailMesh.h"

class CosineEnemy : public Enemy
{
public:
	CosineEnemy(glm::vec3 position, MeshRenderer meshRenderer);

	void update(float deltaTime) override;

	Physicsbody physicsBody;

private:
	Camera* camera;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};