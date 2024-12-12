#include "DefaultCard.h"
#include "../../game.h"
#include <MeshManager.h>
#include <ShaderManager.h>

DefaultCard::DefaultCard(glm::vec3 position)
	: Card(position)
{
	meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), game->camera);
}

void DefaultCard::update(float deltaTime)
{
	glm::vec3 rightVector = glm::cross(launchDirection, upDirection);

	transform.position += launchDirection * deltaTime * 10.f;

	glm::quat yawRotate = glm::angleAxis(glm::radians(deltaTime * 900.f), -upDirection);
	transform.rotation = yawRotate * transform.rotation;


	Entity* hit_actor = game->get_colliding_entity_OBB(this, Collision_Channel::Enemy);
	if (hit_actor != nullptr)
	{
		Enemy* enemy = dynamic_cast<Enemy*>(hit_actor);
		if (enemy != nullptr)
		{
			enemy->take_damage(damage * game->playerDamageMultiplier);
			
			game->spawn_particle_source(transform.position, 20.0f, 0.3f, 0.8f);

			if(bDestroyOnHit)
				destroy();
		}
	}

	timer.updateTimer(deltaTime);
}

void DefaultCard::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	bDestroyOnHit = true;
	aliveTime = 10.f;
	damage = 5.f;

	initializeTimer(aliveTime);
	Card::launch(launchPosition, launchDirection, upDirection);
	transform.lookAt(transform.position + launchDirection, upDirection);
}
