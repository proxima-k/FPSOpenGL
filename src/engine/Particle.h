#pragma once

#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "../graphics/MeshRenderer.h"
#include <glm/glm.hpp>

class Particle
{
public:
	Particle(glm::vec3 startPos, glm::vec3 scale, Mesh* mesh, Shader* shader, Camera* camera);
	~Particle();

	void render();

	void isDestroyed();

private:
	MeshRenderer* meshRenderer;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	float duration;

	float speed;
};