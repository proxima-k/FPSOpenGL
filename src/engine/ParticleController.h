#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "Particle.h"

class ParticleController
{
public:
	ParticleController(glm::vec3 startPos) {
		std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("res/models/cube.obj");
		mesh = new Mesh(vertices);

		shader = new Shader("res/shaders/Basic.shader");

		std::cout << mesh->getVerticesCount() << std::endl;

		for (int i = 0; i < 2; i++) {
			particles[i] = new Particle(startPos, mesh, shader, Camera::mainCamera);
		}

		std::cout << "Particle Controller Spawned at -> " << " X: " << startPos.x << " Y: " << startPos.y << " Z: " << startPos.z << std::endl;
	}

	void render()
	{
		for(auto p : particles) {
			if(p != nullptr)
			p->render();
		}

		std::cout << "Render" << std::endl;
	}

	bool isEmpty() {
		return true;
	}

private:
	Particle* particles[100];
	Shader* shader;
	Mesh* mesh;
};