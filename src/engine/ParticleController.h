#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "Particle.h"

class ParticleController
{
public:
	ParticleController(glm::vec3 startPos, const int particleAmount, const float duration, const float speed);

	void render();
	void update(float dt);

	bool isEmpty() {
		for (int i = 0; i < 100; i++)
		{
			auto particle = particles[i];

			if (particle != nullptr) {
				return false;
			}
		}

		return true;
	}

private:
	Particle* particles[100] = { nullptr };
	Shader* shader = nullptr;
	Mesh* mesh = nullptr;
};