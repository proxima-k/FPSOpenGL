#include "Card.h"

Card::Card(glm::vec3 position)
	: Entity(position)
{
	collision_channel = Collision_Channel::Projectile;
}

void Card::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	this->launchPosition = launchPosition;
	this->launchDirection = launchDirection;
	this->upDirection = upDirection;

	initializeTimer(aliveTime);
}

void Card::initializeTimer(float time)
{
	timer.setTimer(time);
	timer.setCallback([this]() { destroy(); });
	timer.startTimer();
}

void Card::update(float deltaTime)
{
	transform.position += launchDirection * 10.f * deltaTime;
	timer.updateTimer(deltaTime);
}
