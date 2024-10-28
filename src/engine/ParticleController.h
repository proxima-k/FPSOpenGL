#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "Particle.h"

class ParticleController
{
public:
	ParticleController(glm::vec3 startPos) {
		std::cout << "Particle Controller Spawned at -> " << " X: " << startPos.x << " Y: " << startPos.y << " Z: " << startPos.z << std::endl;

		std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("../../res/models/cube.obj");
		Mesh cubeMesh(vertices);

		Shader meshShader("../../res/shaders/Basic.shader");

		for (int i = 0; i < 100; i++) {
			particles[i] = new Particle(startPos, &cubeMesh, &meshShader, Camera::mainCamera);
		}
	}

	void render()
	{
		for(auto p : particles) {
			p->render();
		}

		std::cout << "Render" << std::endl;
	}

	bool isEmpty() {
		return true;
	}

private:
	Particle* particles[100];
};