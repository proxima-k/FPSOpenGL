#include "ParticleController.h"

#include "../game/Game.h"

ParticleController::ParticleController(glm::vec3 startPos)
{
	std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("res/models/cube.obj");
	mesh = new Mesh(vertices);

	shader = new Shader("res/shaders/Basic.shader");

	std::cout << "Vertex count : " << mesh->getVerticesCount() << std::endl;

	for (int i = 0; i <= 2; i++) {
		particles[i] = new Particle(startPos, glm::vec3(0.1f), mesh, shader, Camera::mainCamera);
	}

	std::cout << "Particle Controller Spawned at -> " << " X: " << startPos.x << " Y: " << startPos.y << " Z: " << startPos.z << std::endl;
}

void ParticleController::render()
{
	for (auto p : particles) {
		if (p != nullptr)
			p->render();
	}
}
