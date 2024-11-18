#include "BossHealingCube.h"

#include "../../Game.h"


BossHealingCube::BossHealingCube(glm::vec3 spawnPosition, glm::vec3 targetPosition, HealTask* healTaskNode)
	: Enemy(spawnPosition), healTaskNode(healTaskNode)
{
	healingLine = game->spawn_healing_line(spawnPosition, targetPosition);
}

BossHealingCube::~BossHealingCube()
{
	healingLine->destroy();
}