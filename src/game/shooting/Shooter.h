#pragma once

#include "glm/glm.hpp"
#include "../../graphics/Mesh.h"
#include "../../graphics/Shader.h"
#include "../Camera.h"

class Shooter
{
public:
	Shooter();
	void setCardRenderer(Mesh* cardMesh, Shader* cardShader, Camera* camera);

	void shoot(glm::vec3 launchPosition, glm::vec3 launchDirection);

private:	
	Mesh* cardMesh;
	Shader* cardShader;
	Camera* camera;
};