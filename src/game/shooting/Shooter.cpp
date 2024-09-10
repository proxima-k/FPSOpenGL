#include "Shooter.h"

#include "../Game.h"
#include "cards/SineCard.h"
#include "cards/CosineCard.h"
#include "cards/DefaultCard.h"
#include "UI/UICard.h"
#include "../Player.h"

Shooter* shooter = nullptr;

Shooter::Shooter() 
{
	shooter = this;

	// temporary add cards to queue to test UI
	cardQueue.push(new DefaultCard(glm::vec3(0), MeshRenderer(cardMesh, cardShader, camera)));
	cardQueue.push(new CosineCard(glm::vec3(0), MeshRenderer(cardMesh, cardShader, camera)));
	cardQueue.push(new SineCard(glm::vec3(0), MeshRenderer(cardMesh, cardShader, camera)));
}

void Shooter::spawnCard(Card* card, glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) {
	MeshRenderer newMeshRenderer(cardMesh, cardShader, camera);

	Card* newCard = nullptr;

	switch (card->getCardType()) {
	case Card::Normal:
		newCard = game->spawn_entity<DefaultCard>(launchPosition, newMeshRenderer);
		break;
	case Card::Cosine:
		newCard = game->spawn_entity<CosineCard>(launchPosition, newMeshRenderer);
		break;
	case Card::Sine:
		newCard = game->spawn_entity<SineCard>(launchPosition, newMeshRenderer);
		break;
	}

	if (newCard != nullptr) {
		newCard->transform.scale = glm::vec3(0.1f, 0.005f, 0.1f);
		newCard->launch(launchPosition, launchDirection, upDirection);
	}
}

void Shooter::setCardRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera)
{
	this->cardMesh = cardMesh;
	this->cardShader = cardShader;
	this->camera = camera;
}

void Shooter::shootCardFromQueue(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	if (cardQueue.size() <= 0) return;

	MeshRenderer newMeshRenderer(cardMesh, cardShader, camera, glm::vec3(0.3f, 1.f, 0.3f));
	Card* cardToSpawn = cardQueue.front();

	spawnCard(cardToSpawn, launchPosition, launchDirection, upDirection);

	cardQueue.pop();
	cardQueue.push(cardToSpawn);
}

void Shooter::shootDefault(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	MeshRenderer newMeshRenderer(cardMesh, cardShader, camera, glm::vec3(0.3f, 1.f, 0.3f));

	DefaultCard* defaultCard = new DefaultCard(launchPosition, newMeshRenderer);

	spawnCard(defaultCard, launchPosition, launchDirection, upDirection);

	delete defaultCard;
}



