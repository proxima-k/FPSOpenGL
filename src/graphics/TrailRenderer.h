#pragma once

#include "TrailMesh.h"
#include "Shader.h"
#include "../game/Camera.h"

class TrailRenderer
{
public:
	TrailRenderer();
	TrailRenderer(TrailMesh* mesh, Shader* shader, Camera* camera, glm::vec3 color = glm::vec3(1.f));

	void draw(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

	void setMesh(TrailMesh* mesh) { this->mesh = mesh; }
	void setShader(Shader* shader) { this->shader = shader; }
	void setTargetCamera(Camera* camera) { this->targetCamera = camera; }
	void setColor(glm::vec3 color) { this->meshColor = color; }

	TrailMesh* getMesh() { return this->mesh; }

private:
	TrailMesh* mesh = nullptr;
	Shader* shader = nullptr;
	Camera* targetCamera = nullptr;
	glm::vec3 meshColor = glm::vec3(1);
};

