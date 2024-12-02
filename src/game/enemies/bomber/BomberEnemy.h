#pragma once

#include "../../Enemy.h"
#include "../../Physicsbody.h"
#include "../../Player.h"
#include "../../../graphics/TrailMesh.h"

class BomberEnemy : public Enemy
{
public:
	BomberEnemy(glm::vec3 position);

	void update(float deltaTime) override;

	Physicsbody physicsBody;

private:
	Camera* camera;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;

	float pulsationIncreaseIncrement = 1.5f;
	float pulsationSpeed = 0.0f;
	float pulsationTime = 0.0f;

	glm::vec3 initialScale;
};