#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "../game/Camera.h"

class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Mesh* mesh, Shader* shader, Camera* camera, glm::vec3 color = glm::vec3(1.f));

	void draw(glm::vec3 position, glm::quat rotation, glm::vec3 scale);
	
	void setMesh(Mesh* mesh) { this->mesh = mesh; }
	void setShader(Shader* shader) { this->shader = shader; }
	void setTargetCamera(Camera* camera) { this->targetCamera = camera; }
	void setColor(glm::vec3 color) { this->meshColor = color; }

private:
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;
	Camera* targetCamera = nullptr;
	glm::vec3 meshColor;
};

