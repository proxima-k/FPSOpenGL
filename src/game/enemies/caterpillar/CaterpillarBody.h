#pragma once

#include "../../Enemy.h"
#include "../../Physicsbody.h"
#include "../../Player.h"

#include "../../../graphics/TrailMesh.h"

class CaterPillarBody : public Enemy
{
public:
	CaterPillarBody(glm::vec3 position);

	void follow_piece(Enemy* enemy, float dt);

	Physicsbody physicsBody;

private:
	Camera* camera;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};