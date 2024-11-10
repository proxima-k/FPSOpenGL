#include "BodyRotateTask.h"
#include "../../Entity.h"

//#include <glm/glm.hpp>
//#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <random>

void BodyRotateTask::onNodeStart(BT::Blackboard& blackboard)
{
    body = blackboard.getValue<Entity*>("body");
    if (body == nullptr) {
        state = BT::NodeState::FAILURE;
        return;
    }

    std::cout << "Body Rotate starting" << std::endl;
    std::random_device rd;
    std::mt19937 gen(rd());
    // Define a discrete distribution that produces -1 or 1 with equal probability
    std::uniform_int_distribution<int> dis1(0, 1);
    std::uniform_int_distribution<int> dis2(1, 3);

    glm::vec3 axisToRotate;
    switch (dis2(gen)) {
        case 1:
            axisToRotate = glm::vec3(1.f, 0.f, 0.f);
            break;
        case 2:
            axisToRotate = glm::vec3(0.f, 1.f, 0.f);
            break;
        case 3:
            axisToRotate = glm::vec3(0.f, 0.f, 1.f);
            break;
        default:
            axisToRotate = glm::vec3(0.f, 1.f, 0.f);
            break;
    }
    //std::cout << axisToRotate.x << " " << axisToRotate.y << " " << axisToRotate.z << std::endl;

    targetRotation = glm::angleAxis(glm::radians(dis1(gen) == 0 ? 90.f : -90.f), axisToRotate) * body->transform.rotation;
    // calculate target quaternion rotation

    //state = BT::NodeState::SUCCESS;
}

BT::NodeState BodyRotateTask::onNodeUpdate(float deltaTime, BT::Blackboard& blackboard)
{
    if (body == nullptr) {
        return BT::NodeState::FAILURE;
    }

    glm::quat currentRotation = body->transform.rotation;
    currentRotation = glm::slerp(currentRotation, targetRotation, 0.09f);
    body->transform.rotation = currentRotation;

    std::cout << glm::dot(currentRotation, targetRotation) << std::endl;
    if (glm::dot(currentRotation, targetRotation) >= 0.99999f) {
        std::cout << "TARGET ROTATION REACHED PUREEEEEESSSSSSSUUUU STOPPPP" << std::endl;
        body->transform.rotation = targetRotation;
        return BT::NodeState::SUCCESS;
    }


    return BT::NodeState::RUNNING;
}

void BodyRotateTask::onNodeFinish(BT::Blackboard& blackboard)
{
    std::cout << "Body Rotate finished" << std::endl;
}
