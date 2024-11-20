#include "HealTask.h"

#include "../../Game.h"
#include "BossCage.h"
#include "BossEnemy.h"
#include "BossHealingCube.h"

void HealTask::notifyHealingCubeDeath(int index)
{
    currentHealingCubeCount--;
    healingCubes[index]->destroy();
    healingCubes[index] = nullptr;
}

void HealTask::onNodeStart(BT::Blackboard& blackboard)
{
    BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
    if (boss != nullptr)     
        boss->setCanCollide(false);
    
    std::cout << "Start healing" << std::endl;

    // get random cell on the wall grid, spawn a cube
    // assign cube's target location
    // form a line towards the target location
    // cube will move towards the target location
    BossCage* bossCage = boss->bossCage;

    if (bossCage == nullptr) return;

    currentHealingCubeCount = MAX_HEALING_CUBE_COUNT;
    timer = MAX_HEALING_TIME;
    healingCubes.clear();

    for (int i = 0; i < MAX_HEALING_CUBE_COUNT; i++) {
        std::cout << i << std::endl;
        glm::vec3 spawnPosition, cellNormal;
        spawnPosition = bossCage->getCellCenterCoords(i * (360.f / MAX_HEALING_CUBE_COUNT), 4, cellNormal);
    
        BossHealingCube* newHealingCube = new BossHealingCube(spawnPosition, glm::vec3(0), this, i);
        game->add_entity(newHealingCube);
        healingCubes.push_back(newHealingCube);
    }

}

BT::NodeState HealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{

    if (currentHealingCubeCount <= 0) return BT::NodeState::FAILURE;
    if (timer <= 0) return BT::NodeState::SUCCESS; 

    timer -= deltaTime;

    return BT::NodeState::RUNNING;
}

void HealTask::onNodeFinish(BT::Blackboard& blackboard)
{

    BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
    if (boss != nullptr);
        boss->setCanCollide(true);

    std::cout << "Finished " << std::endl;
    int healAttempts = blackboard.getValue<int>("healAttempts") - 1;
    blackboard.setValue<int>("healAttempts", healAttempts);

    // if state is success, call add body in boss brain
    // if state is failure, then destroy the body

    for (int i = 0; i < healingCubes.size(); i++) {
        if (healingCubes[i] == nullptr) continue;
        
        healingCubes[i]->destroy();
        healingCubes[i] = nullptr;
    }
}
