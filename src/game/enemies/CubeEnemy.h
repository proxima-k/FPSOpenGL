#pragma once

#include "../Enemy.h"
#include "../Physicsbody.h"
#include "../Player.h"
#include "../../graphics/TrailMesh.h"

class CubeEnemy : public Enemy 
{
public:
	CubeEnemy(glm::vec3 position);

	void update(float deltaTime) override;

	Physicsbody physicsBody;

private:
	Camera* camera;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};