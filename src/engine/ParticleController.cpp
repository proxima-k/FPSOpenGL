#include "ParticleController.h"

#include "../game/Game.h"

ParticleController::ParticleController(glm::vec3 startPos, const int particleAmount, const float duration, const float speed)
{
	std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("res/models/cube.obj");
	mesh = new Mesh(vertices);
	shader = new Shader("res/shaders/mesh.shader");

	for (int i = 0; i <= particleAmount; i++) {
		glm::vec3 scale = glm::vec3(0.01f);
		particles[i] = new Particle(startPos, scale, duration, speed, mesh, shader, Camera::mainCamera);
	}
}

void ParticleController::render()
{
	for (int i = 0; i < 100; i++)
	{
		auto particle = particles[i];
		if (particle == nullptr) continue;

		if (particle->isDestroyed())
		{
			particles[i] = nullptr;
			delete particle;
			continue;
		}
		particle->render();
	}
}

void ParticleController::update(float dt)
{
	for (auto p : particles) {

		if (p == nullptr) continue;

		p->update(dt);
	}
}
