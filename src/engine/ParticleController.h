#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "Particle.h"

class ParticleController
{
public:
	ParticleController(glm::vec3 startPos);

	void render();

	bool isEmpty() {
		return true;
	}

private:
	Particle* particles[100] = { nullptr };
	Shader* shader = nullptr;
	Mesh* mesh = nullptr;
};