#include "HealTask.h"

#include "../../Game.h"
#include "BossCage.h"
#include "BossEnemy.h"
#include "BossHealingCube.h"
#include "BossBody.h"



void HealTask::onNodeStart(BT::Blackboard& blackboard)
{
    BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
    BossCage* bossCage = boss->bossCage;
    bodyIndex = boss->getRandomEmptyIndex();
    if (boss == nullptr || bossCage == nullptr || bodyIndex == -1) {
        state = BT::NodeState::FAILURE;
        return;
    }

    boss->setCanCollide(false);

    // spawning new boss body
    glm::vec3 offsetDirection = boss->getOffsetDirectionFromIndex(bodyIndex);
    newBossBody = new BossBody(offsetDirection, bodyIndex, offsetDirection, boss->getOffsetMagnitude());
    newBossBody->setBossController(boss);
    newBossBody->collision_channel = Collision_Channel::None;
    newBossBody->smoothSizing = true;
    newBossBody->health = 0;
    game->add_entity(newBossBody);
    boss->notifyBossBodyRegenerate(newBossBody, bodyIndex);
    boss->setTargetColor(glm::vec3(1.f));

    currentHealingCubeCount = MAX_HEALING_CUBE_COUNT * (4 - blackboard.getValue<int>("healAttempts"));
    currentTotalHealingCubes = currentHealingCubeCount;
    healingCubesReadyCount = 0;
    timer = MAX_HEALING_TIME;
    healingCubes.clear();

    for (int i = 0; i < currentHealingCubeCount; i++) {
        glm::vec3 spawnPosition, cellNormal;
        spawnPosition = bossCage->getCellCenterCoords(i * (360.f / currentHealingCubeCount), 2, cellNormal) + bossCage->centerWorldPosition;
    
        BossHealingCube* newHealingCube = new BossHealingCube(spawnPosition, newBossBody, this, i, cellNormal);
        game->add_entity(newHealingCube);
        healingCubes.push_back(newHealingCube);
    }

    colorTimer = 0.f;
}

BT::NodeState HealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    if (healingCubesReadyCount < currentTotalHealingCubes) return BT::NodeState::RUNNING;

    if (currentHealingCubeCount <= 0) return BT::NodeState::FAILURE;
    if (timer <= 0) return BT::NodeState::SUCCESS; 

    newBossBody->health = newBossBody->maxHealth * (1 - timer/MAX_HEALING_TIME);
    timer -= deltaTime;

    if (colorTimer < 0) {
        colorTimer = timeToSwapColor;

        BossEnemy* boss = blackboard.getValue<BossEnemy*>("boss");
        boss->setTargetColor(getColorFromIndex(colorIndex));

        colorIndex+=2;
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

void HealTask::notifyHealingCubeReady()
{
    healingCubesReadyCount++;

    if (healingCubesReadyCount >= currentTotalHealingCubes) {
        for (int i = 0; i < currentTotalHealingCubes; i++) {
            healingCubes[i]->collision_channel = Collision_Channel::Enemy;
        }
    }
}

glm::vec3 HealTask::getColorFromIndex(int index)
{
    switch (index) {
    case 0:
        return glm::vec3(1.0f, 0.3f, 0.3f);
    case 1:
        return glm::vec3(1.0f, 1.0f, 0.3f);
    case 2:
        return glm::vec3(0.698f, 0.992f, 0.2f);
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