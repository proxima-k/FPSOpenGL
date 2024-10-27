#pragma once

#include "../Enemy.h"
#include "../Physicsbody.h"
#include "../Player.h"
#include "../../graphics/TrailMesh.h"

class CubeEnemy : public Enemy 
{
public:
	enum Variants
	{
		Basic,
		Cosine,
		Sine,
	};


	CubeEnemy(glm::vec3 position, MeshRenderer meshRenderer);

	void update(float deltaTime) override;

	Variants chooseVariant();
	Variants enemyType;

	Physicsbody physicsBody;

private:
	Camera* camera;
	TrailRenderer trailRenderer;

	glm::vec3 lastPosition;
};