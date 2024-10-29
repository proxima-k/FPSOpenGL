#include "BossEnemy.h"

void BossEnemy::update(float deltaTime)
{
	behaviorTree.update(deltaTime);
}
