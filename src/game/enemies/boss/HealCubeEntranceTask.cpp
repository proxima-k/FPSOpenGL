#include "HealCubeEntranceTask.h"

#include <glm/glm.hpp>

#include "HealTask.h"
#include "BossHealingCube.h"
#include "../game/AudioManager.h"

#include <iostream>

HealCubeEntranceTask::HealCubeEntranceTask(float offset)
{
	timer = offset;
	//std::cout << timer << std::endl;
}

void HealCubeEntranceTask::onNodeStart(BT::Blackboard& blackboard)
{
	healCube = blackboard.getValue<BossHealingCube*>("healCube");
	healCube->collision_channel = Collision_Channel::None;
}

BT::NodeState HealCubeEntranceTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
	if (timer > 0) {
		timer -= deltaTime;

		return BT::NodeState::RUNNING;
	}
	else {
		if (!bPlaySpawnSound) {
			game->camera->invokeScreenShake(0.05f, 0.25f);
			auto clip = audioManager->getAudioClip("pillarThud.mp3");
			if (clip)
				audioManager->playSound(clip, glm::vec3(0), 0.25f);

			bPlaySpawnSound = true;
		}
	}

	if (t >= 0.99f) return BT::NodeState::SUCCESS;

	glm::vec3 cellNormal = blackboard.getValue<glm::vec3>("cellNormal");
	glm::vec3 cellPosition = blackboard.getValue<glm::vec3>("cellPosition");
	glm::vec3 currentScale = glm::abs(cellNormal) * t + (glm::vec3(1.f) - glm::abs(cellNormal));
	healCube->transform.position = cellPosition + (cellNormal * t / 2.f);
	healCube->transform.scale = currentScale;

	t = glm::mix(t, 1.0f, 0.08f);

	return BT::NodeState::RUNNING;
}

void HealCubeEntranceTask::onNodeFinish(BT::Blackboard& blackboard)
{
	//healCube->collision_channel = Collision_Channel::Enemy;
	
	glm::vec3 cellNormal = blackboard.getValue<glm::vec3>("cellNormal");
	glm::vec3 cellPosition = blackboard.getValue<glm::vec3>("cellPosition");
	healCube->transform.position = cellPosition + (cellNormal / 2.f);
	healCube->transform.scale = glm::vec3(1.f);

	HealTask* task = blackboard.getValue<HealTask*>("healTaskNode");
	task->notifyHealingCubeReady();

	healCube->spawnHealingLine();
}
