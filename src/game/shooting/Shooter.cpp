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
}

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

void Shooter::shootCardFromQueue(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	MeshRenderer newMeshRenderer(cardMesh, cardShader, camera, glm::vec3(0.3f, 1.f, 0.3f));
	//		spawnCard<SineCard>(launchPosition, launchDirection, upDirection);
}

void Shooter::setupUI() {
	glm::vec3 offset = player->transform.position + player->transform.getForward();
}

void Shooter::shootDefault(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection)
{
	MeshRenderer newMeshRenderer(cardMesh, cardShader, camera, glm::vec3(0.3f, 1.f, 0.3f));

	spawnCard<DefaultCard>(launchPosition, launchDirection, upDirection);
}

