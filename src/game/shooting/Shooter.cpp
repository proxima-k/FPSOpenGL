#include "Shooter.h"

#include "../Game.h"
#include "cards/SineCard.h"
#include "cards/CosineCard.h"
#include "UI/UICard.h"
#include "../Player.h"

Shooter::Shooter() {}

void Shooter::update(float deltaTime)
{
	for (Entity* uiCard : uiCards) {
		uiCard->transform.position = player->getCamera()->transform.position + player->getCamera()->transform.getForward();
		//uiCard->transform.position = glm::vec3(1.f);
		glm::vec3 faceDir = player->getCamera()->transform.getForward();
		uiCard->transform.rotation = glm::quatLookAt(-faceDir, glm::vec3(0,1,0));

		uiCard->update(deltaTime);
	}
}

void Shooter::setCardRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera)
{
	this->cardMesh = cardMesh;
	this->cardShader = cardShader;
	this->camera = camera;
}

void Shooter::shoot(glm::vec3 launchPosition, glm::vec3 launchDirection)
{
	MeshRenderer newMeshRenderer(cardMesh, cardShader, camera, glm::vec3(0.3f, 1.f, 0.3f));

	int cardType = rand() % 2;

	// spawn a card and launch it
	switch (cardType) {
		case 0:
			spawnCard<SineCard>(launchPosition, launchDirection);
			break;
		case 1:
			spawnCard<CosineCard>(launchPosition, launchDirection);
			break;
	}
}

void Shooter::setupUI() {
	glm::vec3 offset = player->transform.position + player->transform.getForward();
	//glm::vec3 offset(1.f);

	/*for (int i = 0; i < 2; i++) {
		UICard* newUICard = game->spawn_entity<UICard>(offset, MeshRenderer(cardMesh, cardShader, camera, glm::vec3(1.0f, 0.3f, 0.3f)));
		newUICard->transform.scale = glm::vec3(0.1f, 0.1f, 0.001f);
		uiCards.push_back(newUICard);
	}*/
}

