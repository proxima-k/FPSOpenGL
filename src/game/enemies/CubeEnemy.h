#pragma once

#include "../Enemy.h"
#include "../Physicsbody.h"
#include "../Player.h"

class CubeEnemy : public Enemy 
{
public:
	CubeEnemy(glm::vec3 position, MeshRenderer meshRenderer);

	void update(float deltaTime) override;

	Physicsbody physicsBody;

private:
	Camera* camera;
};