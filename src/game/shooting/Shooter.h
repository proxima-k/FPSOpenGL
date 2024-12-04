#pragma once

#include <vector>
#include <queue>

#include "glm/glm.hpp"
#include "../../graphics/Mesh.h"
#include "../../graphics/Shader.h"
#include "../Camera.h"
#include "../Entity.h"
#include "../Game.h"
#include "Card.h"

class Player;

class Shooter
{
public:
	Shooter();

	void shootCardFromQueue(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);
	void shootDefault(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);
	void spawnCard(Card* card, glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);

	void emptyAllQueues();

	void setCardRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);
	void setPlayer(Player* player) { this->player = player; }

	void update(float deltaTime);
	float getCardCooldownLeftPercentage() { return currentCardCooldown / CARD_COOLDOWN; }

	std::queue<Card*> cardQueue;
	std::queue<Card*> cardPassivesQueue;

	Mesh* cardMesh;
	Shader* cardShader;
	Camera* camera;


private:	
	float currentCardCooldown = 0.f;
	const float CARD_COOLDOWN = 0.5f;

	std::vector<Entity*> uiCards;

	Player* player;
};

extern Shooter* shooter;