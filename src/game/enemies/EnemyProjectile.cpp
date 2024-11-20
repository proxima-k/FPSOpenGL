#include "EnemyProjectile.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include "../Game.h"

EnemyProjectile::EnemyProjectile(glm::vec3 position)
{
	meshRenderer = new MeshRenderer(meshManager->getMesh("cone"), shaderManager->getShader("mesh"), game->camera);
	meshRenderer->setColor( { 1, 0, 0 } );
	transform.scale *= 0.1f;
	projectileSpeed = 8.0f;
}

void EnemyProjectile::update(float dt) {
	transform.position += transform.getForward() * glm::vec3(projectileSpeed) * dt;
}

void EnemyProjectile::launch(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	aliveTime = 200.0f;

	transform.position = launchPosition;
	transform.rotation = glm::quatLookAt(glm::normalize(launchDirection), glm::normalize(upDirection));
}