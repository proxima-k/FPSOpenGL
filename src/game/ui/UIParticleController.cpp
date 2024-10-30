#include "UIParticleController.h"

#include <ostream>
#include <iostream>

UIParticleController::UIParticleController(ImVec2 startPos, int particleCount, int particleDuration) {
	currentTime = particleDuration;

	uiParticles.resize(particleCount);

	for (int i = 0; i < particleCount; ++i)
	{
		UIParticle* particle = new UIParticle(startPos);
		uiParticles.push_back(particle);

		std::cout << "Particle Spawned" << std::endl;
	}
}

void UIParticleController::update(float dt)
{
	currentTime -= dt;

	if (currentTime <= 0) { bDestroyed = true; return; }

	for (int i = 0; i < uiParticles.size(); i++)
	{
		auto particle = uiParticles[i];

		if (particle != nullptr)
		{
			particle->update(dt);
		}
	}
}

void UIParticleController::render()
{
	for (int i = 0; i < uiParticles.size(); i++)
	{
		auto particle = uiParticles[i];

		if (particle != nullptr)
		{
			particle->render();
		}
	}
}
