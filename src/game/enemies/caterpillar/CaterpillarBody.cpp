#include "CaterPillarBody.h"

#include <iostream>
#include <MeshManager.h>
#include <ShaderManager.h>

#include "../../Game.h"

CaterPillarBody::CaterPillarBody(glm::vec3 position)
    : Enemy(position), physicsBody(), trailRenderer()
{
    entityName = "Body";

    owner = nullptr;
    collision_channel = Collision_Channel::Enemy;

    xpAmount = 1.0f;
    maxHealth = 10.0f;
    health = maxHealth / 2;

	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
}

void CaterPillarBody::update(float deltaTime)
{
    if (owner != nullptr) {
        std::cout << owner->entityName << std::endl;
        glm::vec3 targetPos = owner->transform.position; // Get the position of the owner

        std::cout << "Owner position: " << targetPos.x << ", " << targetPos.y << ", " << targetPos.z << std::endl;

        float minDistance = 0.1f;
        float currentDistance = glm::distance(transform.position, targetPos);

        if (currentDistance > minDistance) {
            float followSpeed = 5.0f;
            transform.position = glm::mix(transform.position, targetPos, followSpeed * deltaTime);
            transform.position.y = owner->transform.position.y; // Match the Y position

            glm::quat targetRotation = glm::quatLookAt(glm::normalize(targetPos - transform.position), glm::vec3(0, 1, 0));
            float rotationSpeed = 3.0f;
            transform.rotation = glm::slerp(transform.rotation, targetRotation, rotationSpeed * deltaTime);
        }
    }
    else {
        std::cout << "Owner is not set." << std::endl;
    }
}
