#include "PlaceHolderCard3.h"

void PlaceHolderCard3::update(float deltaTime)
{
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

    float orbitSpeed = 5.f;
    angle += orbitSpeed * deltaTime;

    if (angle > glm::two_pi<float>()) {
        angle -= glm::two_pi<float>();
    }

    float x = playerPosition.x + orbitRadius * cos(angle);
    float z = playerPosition.z + orbitRadius * sin(angle);

    glm::vec3 cameraForward = glm::normalize(game->camera->transform.getForward());
    float verticalOffset = cameraForward.y * orbitRadius;

    transform.position = glm::vec3(x, playerPosition.y + verticalOffset, z);

    glm::vec3 direction = glm::normalize(transform.position - playerPosition);
    transform.rotation = glm::quatLookAt(direction, upDirection);

    playerPosition = game->camera->transform.position;
}

void PlaceHolderCard3::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = false;
	damage = 10;
	aliveTime = 200.0f;

	meshRenderer.setColor({ 0, 0, 0 });
	transform.scale = glm::vec3(0.1f, 0.005f, 0.1f);

	playerPosition = launchPosition;

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	currentPosition = launchPosition;

	//Initial passives
	transform.scale *= game->playerCardSizeMultiplier;
}
