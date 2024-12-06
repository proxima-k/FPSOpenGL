#include "Shooter.h"

#include "../Game.h"
#include "cards/SineCard.h"
#include "cards/CosineCard.h"
#include "cards/PlaceholderCard1.h"
#include "cards/PlaceholderCard2.h"
#include "cards/PlaceholderCard3.h"
#include "cards/DefaultCard.h"
#include "cards/DamagePassive.h"
#include "UI/UICard.h"
#include "../Player.h"

Shooter* shooter = nullptr;

Shooter::Shooter()
{
	shooter = this;
}

Card* Shooter::spawnCard(Card::CardType cardType, glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) {
	Card* newCard = nullptr;

	switch (cardType) {
	case Card::Normal:
		newCard = game->spawn_entity<DefaultCard>(launchPosition);
		break;
	case Card::Cosine:
		newCard = game->spawn_entity<CosineCard>(launchPosition);
		break;
	case Card::Sine:
		newCard = game->spawn_entity<SineCard>(launchPosition);
		break;
	case Card::Placeholder1: // x3
		newCard = game->spawn_entity<PlaceHolderCard1>(launchPosition);
		break;
	case Card::Placeholder2: // pull
		newCard = game->spawn_entity<PlaceHolderCard2>(launchPosition);
		break;
	case Card::Placeholder3: // circle
		newCard = game->spawn_entity<PlaceHolderCard3>(launchPosition);
		break;
	}

	if (newCard != nullptr) {
		newCard->launch(launchPosition, launchDirection, upDirection);
	}

	return newCard;
}

void Shooter::emptyAllQueues() {
	while (!cardQueue.empty()) cardQueue.pop();
	while (!cardPassivesQueue.empty()) cardPassivesQueue.pop();
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

	if (currentCardCooldown > 0) return;

	Card::CardType cardToSpawn = cardQueue.front();

	spawnCard(cardToSpawn, launchPosition, launchDirection, upDirection);

	cardQueue.pop();
	cardQueue.push(cardToSpawn);

	currentCardCooldown = CARD_COOLDOWN;
}

void Shooter::shootDefault(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	//DefaultCard* defaultCard = new DefaultCard(launchPosition);

	spawnCard(Card::Normal, launchPosition, launchDirection, upDirection);

	//delete defaultCard;
}

void Shooter::update(float deltaTime) {
	if (currentCardCooldown > 0.f)
		currentCardCooldown -= deltaTime;
	else
		currentCardCooldown = 0.f;
}