#include "GrassRiseAndFallTask.h"

#include "../../Entity.h"
#include <glm/glm.hpp>

void GrassRiseAndFallTask::onNodeStart(BT::Blackboard& blackboard)
{
    currentHeight = MIN_HEIGHT;
    entity = blackboard.getValue<Entity*>("entity");
    if (entity != nullptr) {
        updateHeight();
        entity->collision_channel = Collision_Channel::Enemy;
        entity->meshRenderer->setColor(glm::vec3(0.698f, 0.992f, 0.2f));
    }
    else {
        state = BT::NodeState::FAILURE;
    }
    
    currentState = 0;
    timer = riseTime;
}

BT::NodeState GrassRiseAndFallTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    // 3 states
    switch (currentState) {
    // rise
    case 0:
        currentHeight = glm::mix(MIN_HEIGHT, MAX_HEIGHT,
            (-glm::pow(((1.f - (timer / riseTime)) - 1), 4) + 1));
        updateHeight();
        
        if (timer <= 0) {
            timer = waitTime;
            currentState++;
        }
        break;
    
    // wait
    case 1:
        updateHeight();

        if (timer <= 0) {
            timer = fallTime;
            currentState++;
        }
        break;
    
    // fall
    case 2:
        float tEquation = glm::pow(((timer / fallTime) - 1), 2);
        currentHeight = glm::mix(MAX_HEIGHT, 0.f, tEquation);
        updateHeight();

        if (timer <= 0) {
            return BT::NodeState::SUCCESS;
        }
        break;
    }
    timer -= deltaTime;

    return BT::NodeState::RUNNING;
}

void GrassRiseAndFallTask::onNodeFinish(BT::Blackboard& blackboard)
{
    if (entity != nullptr) {
        entity->destroy();
    }
}

void GrassRiseAndFallTask::updateHeight() {
    if (entity == nullptr) return;

    entity->transform.scale.y = currentHeight;
    entity->transform.position.y = currentHeight/ 2;
}