#pragma once

#include <vector>

#include "UIParticle.h"
#include "imgui/imgui.h"

class UIParticleController
{
public:
	UIParticleController(ImVec2 startPos, int particleCount, int particleDuration);

	void update(float dt);

	void render();

	bool destroyed() { return bDestroyed; }

	std::vector<UIParticle*> uiParticles;

private:
	bool bDestroyed = false;
	float currentTime;
};