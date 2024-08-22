#pragma once

#include <vector>

#include "glm/glm.hpp"
#include "../../graphics/Mesh.h"
#include "../../graphics/Shader.h"
#include "../Camera.h"
#include "../Entity.h"
//#include "../Player.h"

class Player;

class Shooter
{
public:
	Shooter();

	void update(float deltaTime);

	void shoot(glm::vec3 launchPosition, glm::vec3 launchDirection);
	void setupUI();

	void setCardRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);
	void setPlayer(Player* player) { this->player = player; }

private:	
	Mesh* cardMesh;
	Shader* cardShader;
	Camera* camera;

	std::vector<Entity*> uiCards;

	Player* player;
};