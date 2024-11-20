#pragma once

#include "../../Enemy.h"
#include "../../Physicsbody.h"
#include "../../Player.h"
#include "../../../graphics/TrailMesh.h"

enum EnemyState {
	Chase,
	Dodge,
};

class ShootingEnemy : public Enemy 
{
public:
	ShootingEnemy(glm::vec3 position);
	void initMeshRenderer() override;

	void update(float deltaTime) override;

	void chase_player(float dt);
	void dodge_player(float dt);

	std::vector<glm::vec3> get_random_directions();

	Physicsbody physicsBody;
private:
	Camera* camera;
	TrailRenderer trailRenderer;

	EnemyState state = EnemyState::Chase;
	bool filledList = false;

	std::vector<glm::vec3> target_positions;
	glm::vec3 lastPosition;
};