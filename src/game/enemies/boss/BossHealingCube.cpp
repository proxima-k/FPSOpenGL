#include "BossHealingCube.h"

BossHealingCube::BossHealingCube(glm::vec3 spawnPosition, glm::vec3 targetPosition)
	: Enemy(spawnPosition)
{

}

BossHealingCube::~BossHealingCube()
{

}

/*
void BossHealingCube::draw()
{
	if (meshRenderer == nullptr)
	{
		//std::cout << "MeshRenderer is null" << std::endl;
		return;
	}
	meshRenderer->draw(transform);
	
	// also draw the line
	drawHealingLine();
}
*/
