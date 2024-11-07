#pragma once

#include "../graphics/Mesh.h"
#include "../graphics/Shader.h"
#include "../graphics/MeshRenderer.h"
#include <glm/glm.hpp>

class Particle
{
public:
	Particle(glm::vec3 startPos, glm::vec3 scale, const float duration, const float speed, Mesh* mesh, Shader* shader, Camera* camera);
	~Particle() { delete meshRenderer; }

	void render();
	void update(float dt);

	bool isDestroyed() { return bDestroyed; }

private:
	MeshRenderer* meshRenderer;

	glm::vec3 initialScale;

	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	float duration;
	float currentDuration;

	float speed;

	bool bDestroyed = false;
};