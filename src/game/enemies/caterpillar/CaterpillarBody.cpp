#include "CaterPillarBody.h"

#include <iostream>
#include <MeshManager.h>
#include <ShaderManager.h>

#include "../../Game.h"

CaterPillarBody::CaterPillarBody(glm::vec3 position)
    : Enemy(position), physicsBody(), trailRenderer()
{
    entityName = "Body";

    collision_channel = Collision_Channel::Enemy;

    xpAmount = 1.0f;
    maxHealth = 10.0f;
    health = maxHealth / 2;

	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
}

void CaterPillarBody::follow_piece(Enemy* enemy, float dt)
{
    if (enemy != nullptr) {
        glm::vec3 targetPos = enemy->transform.position;
        float minDistance = 0.02f;
        float currentDistance = glm::distance(transform.position, targetPos);

        if (currentDistance > minDistance) {
            float followSpeed = 10.0f;
            transform.position = glm::mix(transform.position, targetPos, followSpeed * dt);
            transform.position.y = enemy->transform.position.y;

            glm::quat targetRotation = glm::quatLookAt(glm::normalize(targetPos - transform.position), glm::vec3(0, 1, 0));
            float rotationSpeed = 3.0f;
            transform.rotation = glm::slerp(transform.rotation, targetRotation, rotationSpeed * dt);
        }
    }
    else {
        std::cout << "Owner is not set." << std::endl;
    }
}
