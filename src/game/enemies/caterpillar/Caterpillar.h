#pragma once

#include "../../Enemy.h"
#include "../../Physicsbody.h"
#include "../../Player.h"
#include "../../../graphics/TrailMesh.h"

#include "CaterpillarBody.h"

class CaterPillar : public Enemy
{
public:
	CaterPillar(glm::vec3 position);

	void update(float deltaTime) override;

	Physicsbody physicsBody;

	const int bodyLength = 5;
	std::vector<CaterPillarBody*> bodyPieces;

private:
	Camera* camera;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};