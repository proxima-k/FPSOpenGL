#include "AttackTask.h"
#include <iostream>
#include "../../Entity.h"

#include "../game/Game.h"
#include "../game/AudioManager.h"

void AttackTask::onNodeStart(BT::Blackboard& blackboard)
{
	currentHeight = 0.01f;
	Entity* entity = blackboard.getValue<Entity*>("entity");
	if (entity != nullptr) {
		entity->transform.scale.y = currentHeight / 2;
		entity->transform.position.y = currentHeight / 2;
		entity->collision_channel = Collision_Channel::Enemy;
		entity->meshRenderer->setColor(glm::vec3(1.f, 0.3f, 0.3f));

		game->camera->invokeScreenShake(.095f, 0.35f);
		auto clip = audioManager->getAudioClip("pillarThud.mp3");
		if (clip)
			audioManager->playSound(clip, glm::vec3(0), 0.25f);
	}
}

BT::NodeState AttackTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	Entity* entity = blackboard.getValue<Entity*>("entity");

	currentHeight = glm::mix(currentHeight, MAX_HEIGHT, 0.1f);
	if (entity != nullptr) {
		entity->transform.scale.y = currentHeight;
		entity->transform.position.y = currentHeight / 2;
	}

	if (MAX_HEIGHT - currentHeight <= 0.0001) {
		return BT::NodeState::SUCCESS;
	}
	return BT::NodeState::RUNNING;
}

void AttackTask::onNodeFinish(BT::Blackboard& blackboard)
{
	Entity* entity = blackboard.getValue<Entity*>("entity");

	if (entity != nullptr) {
		entity->destroy();
	}
}

// update entity position and scale
// something like the following

/*
void PillarEnemy::update(float deltaTime)
{
	waitTimer -= deltaTime;
	if (waitTimer > 0.f)
	{
		return;
	}

	//currentHeight = glm::mix(currentHeight, 2.f, 0.03f);
	if (currentHeight >= 2.f)
	{
		//currentHeight = 0.01f;
		return;
	}
	currentHeight += 10.f * deltaTime;
	updateHeight();

	/*std::cout << "======================" << std::endl;
	std::cout << "current height: " << currentHeight << std::endl;
	std::cout << "transform scale: " << transform.scale.y << std::endl;
	std::cout << "y position: " << transform.position.y << std::endl;
}


void PillarEnemy::updateHeight()
{
	transform.scale.y = currentHeight;
	transform.position.y = currentHeight;
}
*/
