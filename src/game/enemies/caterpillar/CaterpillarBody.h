#pragma once

#include "../../Enemy.h"
#include "../../Physicsbody.h"
#include "../../Player.h"

#include "../../../graphics/TrailMesh.h"

class CaterPillarBody : public Enemy
{
public:
	CaterPillarBody(glm::vec3 position);

	void update(float deltaTime) override;
	void bind_owner(Entity* newOwner) { 
		if (newOwner != nullptr && owner == nullptr) {
			owner = newOwner;
		}
	}

	Physicsbody physicsBody;
	Entity* owner;

private:
	Camera* camera;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};