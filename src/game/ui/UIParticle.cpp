#include "UIParticle.h"

#include "../../engine/TextureManager.h"

UIParticle::UIParticle(ImVec2 startPos) : position(startPos) {
	if (textureManager == nullptr) {
		std::cerr << "Error: textureManager is not initialized!" << std::endl;
		return;
	}
	texture = textureManager->getTexture("popcat");
}

void UIParticle::update(float dt)
{
	position.y += dt * 50;
	position.x += dt * 50;
}

void UIParticle::render()
{
	ImGui::SetCursorPos(position);
	ImVec2 particleSize = { 30, 30 };
	ImGui::Image((void*)(intptr_t)texture, particleSize);
}
