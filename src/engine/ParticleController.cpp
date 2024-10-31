#include "ParticleController.h"

#include "../game/Game.h"

ParticleController::ParticleController(glm::vec3 startPos, const int particleAmount, const float duration)
{
	std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("res/models/cube.obj");
	mesh = new Mesh(vertices);
	shader = new Shader("res/shaders/Basic.shader");

	for (int i = 0; i <= particleAmount; i++) {
		particles[i] = new Particle(startPos, glm::vec3(0.03f), duration, mesh, shader, Camera::mainCamera);
	}
}

void ParticleController::render()
{
	for (auto p : particles) {
		if (p == nullptr) continue;

		if (p->isDestroyed())
		{
			p = nullptr;
			delete p;
			continue;
		}
		p->render();
	}
}

void ParticleController::update(float dt)
{
	for (auto p : particles) {

		if (p == nullptr) continue;

		p->update(dt);
	}
}
