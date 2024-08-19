#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "../game/Camera.h"

class MeshRenderer
{
public:
	MeshRenderer();
	MeshRenderer(Mesh* mesh, Shader* shader, Camera* camera);

	void draw(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	
	void setMesh(Mesh* mesh) { this->mesh = mesh; }
	void setShader(Shader* shader) { this->shader = shader; }
	void setTargetCamera(Camera* camera) { this->targetCamera = camera; }

private:
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;
	Camera* targetCamera = nullptr;
};

