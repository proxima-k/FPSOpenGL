#include "Shooter.h"

#include "../Game.h"

Shooter::Shooter()
{
}

void Shooter::setCardRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera)
{
	this->cardMesh = cardMesh;
	this->cardShader = cardShader;
	this->camera = camera;
}

void Shooter::shoot(glm::vec3 launchPosition, glm::vec3 launchDirection)
{
	MeshRenderer newMeshRenderer(cardMesh, cardShader, camera);

	// spawn a card and launch it
	Card* newCard = game->spawn_entity<Card>(launchPosition, newMeshRenderer);

	if (newCard != nullptr) {
		newCard->transform.scale = glm::vec3(0.1f);
		newCard->launch(launchPosition, launchDirection);
	}
}
