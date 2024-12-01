#pragma once

#include "../Entity.h"
#include "../../engine/Timer.h"

class EnemyProjectile : public Entity {
public:
	EnemyProjectile(glm::vec3 position);

	void update(float deltaTime) override;
	void launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);

	void initializeTimer(float time)
	{
		timer.setTimer(time);
		timer.setCallback([this]() { destroy(); });
		timer.startTimer();
	}

private:
	float projectileSpeed;
	float aliveTime;

	Timer timer;
};