#include "CreditsObject.h"

#include <MeshManager.h>
#include <ShaderManager.h>

#include "Game.h"
#include "Player.h"

CreditObject::CreditObject(glm::vec3 position) 
	: Entity(position)
{
	transform.scale = glm::vec3(1);
	meshRenderer = new MeshRenderer(meshManager->getMesh("credits"), shaderManager->getShader("mesh"), game->camera);
}

void CreditObject::update(float deltaTime)
{
	transform.position -= glm::vec3(0, 1.5f * deltaTime, 0);

	if (transform.position.y < -10)
		game->player->die();
}
