#include "SineCard.h"
#include <iostream>
#include <iomanip>

void SineCard::update(float deltaTime)
{
    timeElapsed += deltaTime * frequency;

    glm::vec3 newPosition = currentPosition + launchDirection * deltaTime * glm::vec3(3);
    float verticalHeight = amplitude * glm::sin(timeElapsed);
    float height = verticalHeight - (verticalHeight / 2);

    newPosition += upDirection * height;

    updateRotation(newPosition);

    transform.position = newPosition;

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

    previousPosition = currentPosition;
    currentPosition = newPosition;
}

void SineCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
    bDestroyOnHit = false;
    aliveTime = 200.0f;
    damage = 10;

    amplitude = 0.1f;
    frequency = 5;

    initializeTimer(aliveTime);
    Card::launch(launchPosition, launchDirection, upDirection);

    currentPosition = launchPosition;
    previousPosition = launchPosition;

    transform.rotation = glm::quatLookAt(glm::normalize(launchDirection), upDirection);
}
