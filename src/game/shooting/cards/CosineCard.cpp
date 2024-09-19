#include "CosineCard.h"

void CosineCard::update(float deltaTime)
{
    timeElapsed += deltaTime * 5;

    glm::vec3 rightVector = glm::cross(launchDirection, upDirection);
    height = glm::sin(timeElapsed) / 6;
    currentPosition += launchDirection * deltaTime * 3.f;
    glm::vec3 newPosition = currentPosition + rightVector * height;

    transform.position = newPosition;

    updateRotation(newPosition);

    Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
    if (hit_actor != nullptr)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(hit_actor);
        if (enemy != nullptr)
        {
            enemy->take_damage(damage * game->playerDamageMultiplier);

            if (bDestroyOnHit)
                destroy();
        }
    }

    timer.updateTimer(deltaTime);
}

void CosineCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
    bDestroyOnHit = false;
    aliveTime = 200.0f;
    damage = 10;

    initializeTimer(aliveTime);
    Card::launch(launchPosition, launchDirection, upDirection);

    currentPosition = launchPosition;
    previousPosition = launchPosition;

    transform.rotation = glm::quatLookAt(glm::normalize(launchDirection), upDirection);
}
