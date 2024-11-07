#pragma once

#include "../Enemy.h"
#include "../Physicsbody.h"
#include "../Player.h"
#include "../../graphics/TrailMesh.h"

class SineEnemy : public Enemy
{
public:
	SineEnemy(glm::vec3 position);

	void update(float deltaTime) override;

	Physicsbody physicsBody;

private:
	Camera* camera = nullptr;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};