#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "../../graphics/Mesh.h"
#include "../../graphics/Shader.h"
#include "../Camera.h"
#include "../Entity.h"
//#include "../Player.h"
#include "../Game.h"

class Player;

class Shooter
{
public:
	Shooter();

	void update(float deltaTime);

	void shootRandom(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);
	void shootDefault(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection);
	void setupUI();

	void setCardRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);
	void setPlayer(Player* player) { this->player = player; }

private:	
	template <typename CardType>
	void spawnCard(glm::vec3 launchPosition, glm::vec3 launchDirection, glm::vec3 upDirection) {
		MeshRenderer newMeshRenderer(cardMesh, cardShader, camera);

		CardType* newCard = game->spawn_entity<CardType>(launchPosition, newMeshRenderer);
		newCard->meshRenderer.setColor(newCard->getMeshColor());

		if (newCard != nullptr) {
			newCard->transform.scale = glm::vec3(0.1f, 0.005f, 0.1f);
			newCard->launch(launchPosition, launchDirection, upDirection);
		}
	}

	Mesh* cardMesh;
	Shader* cardShader;
	Camera* camera;

	std::vector<Entity*> uiCards;

	Player* player;
};