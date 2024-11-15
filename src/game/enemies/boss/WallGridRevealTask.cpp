#include "WallGridRevealTask.h"

#include "../../WallGrid.h"
#include <glm/glm.hpp>

void WallGridRevealTask::onNodeStart(BT::Blackboard& blackboard)
{
    t = 0.f;
    intensity = 1.f;
}

BT::NodeState WallGridRevealTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    WallGrid* wallGrid = blackboard.getValue<WallGrid*>("wallGrid");

    if (wallGrid == nullptr) return BT::NodeState::FAILURE;

    if (t < 0.9999f) {
        t = glm::mix(t, 1.f, 0.1f);
        //t += deltaTime * 1.5;
        if (t > 0.85f)
            intensity -= deltaTime * 1.7;

    }
    else {
        t = 1.f;
        intensity -= deltaTime * 1.5;
    }

    wallGrid->revealHeight = t;
    wallGrid->revealIntensity = intensity;

    if (intensity <= 0)
        return BT::NodeState::SUCCESS;
    // set shader uniforms
    // set height display
    // set intensity

    return BT::NodeState::RUNNING;
}

void WallGridRevealTask::onNodeFinish(BT::Blackboard& blackboard)
{
}
