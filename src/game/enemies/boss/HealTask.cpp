#include "HealTask.h"

#include "../../Game.h"
#include "BossCage.h"
#include "BossEnemy.h"
#include "BossHealingCube.h"
#include "BossBody.h"



void HealTask::onNodeStart(BT::Blackboard& blackboard)
{
    BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
    if (boss == nullptr) {
        state = BT::NodeState::FAILURE;
        return;
    }
    boss->setCanCollide(false);
    
    BossCage* bossCage = boss->bossCage;
    if (bossCage == nullptr) return;

    // find empty slot, get it's offset 
    // get random cell on the wall grid, spawn a cube
    // assign cube's target location
    // form a line towards the target location

    // spawn position, index, offset direction, offsetMagnitude
    bodyIndex = boss->getRandomEmptyIndex();
    if (bodyIndex== -1) {
        state = BT::NodeState::FAILURE;
        return;
    }

    // spawning new boss body
    glm::vec3 offsetDirection = boss->getOffsetDirectionFromIndex(bodyIndex);
    newBossBody = new BossBody(offsetDirection, bodyIndex, offsetDirection, boss->getOffsetMagnitude());
    newBossBody->setBossController(boss);
    newBossBody->collision_channel = Collision_Channel::None;
    newBossBody->smoothSizing = true;
    game->add_entity(newBossBody);
    boss->notifyBossBodyRegenerate(newBossBody, bodyIndex);

    currentHealingCubeCount = MAX_HEALING_CUBE_COUNT;
    timer = MAX_HEALING_TIME;
    healingCubes.clear();

    for (int i = 0; i < MAX_HEALING_CUBE_COUNT; i++) {
        glm::vec3 spawnPosition, cellNormal;
        spawnPosition = bossCage->getCellCenterCoords(i * (360.f / MAX_HEALING_CUBE_COUNT), 4, cellNormal) + bossCage->centerWorldPosition;
    
        BossHealingCube* newHealingCube = new BossHealingCube(spawnPosition, newBossBody, this, i);
        game->add_entity(newHealingCube);
        healingCubes.push_back(newHealingCube);
    }

    colorTimer = 0.f;
}

BT::NodeState HealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    if (currentHealingCubeCount <= 0) return BT::NodeState::FAILURE;
    if (timer <= 0) return BT::NodeState::SUCCESS; 

    newBossBody->health = newBossBody->maxHealth * (1 - timer/MAX_HEALING_TIME);
    timer -= deltaTime;

    if (colorTimer < 0) {
        colorTimer = timeToSwapColor;

        BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
        boss->setTargetColor(getColorFromIndex(colorIndex));

        colorIndex++;
        if (colorIndex > 5) colorIndex = 0;
    }
    colorTimer -= deltaTime;

    return BT::NodeState::RUNNING;
}

void HealTask::onNodeFinish(BT::Blackboard& blackboard)
{
    // consume a heal attempt
    int healAttempts = blackboard.getValue<int>("healAttempts") - 1;
    blackboard.setValue<int>("healAttempts", healAttempts);


    BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
    if (boss == nullptr) return;
    
    // cleaning up
    for (int i = 0; i < healingCubes.size(); i++) {
        if (healingCubes[i] == nullptr) continue;
        
        healingCubes[i]->destroy();
        healingCubes[i] = nullptr;
    }


    // if state is success, call add body in boss brain
    if (state == BT::NodeState::SUCCESS) {
        newBossBody->smoothSizing = false;
        newBossBody->health = newBossBody->maxHealth;
    }
    // if state is failure, then destroy the body
    else if (state == BT::NodeState::FAILURE) {
        boss->notifyBossBodyDeath(bodyIndex);
        newBossBody->destroy();
    }
    
    boss->setCanCollide(true);
    boss->setTargetColor(glm::vec3(1));
}


void HealTask::notifyHealingCubeDeath(int index)
{
    currentHealingCubeCount--;
    healingCubes[index]->destroy();
    healingCubes[index] = nullptr;
}


glm::vec3 HealTask::getColorFromIndex(int index)
{
    switch (index) {
    case 0:
        return glm::vec3(1.0f, 0.3f, 0.3f);
    case 1:
        return glm::vec3(1.0f, 1.0f, 0.3f);
    case 2:
        return glm::vec3(0.3f, 1.0f, 0.3f);
    case 3:
        return glm::vec3(0.3f, 1.0f, 1.0f);
    case 4:
        return glm::vec3(0.3f, 0.3f, 1.0f);
    case 5:
        return glm::vec3(1.0f, 0.3f, 1.0f);
    default:
        return glm::vec3(1);
    }
}