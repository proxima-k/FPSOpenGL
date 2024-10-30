#pragma once

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"

class UIParticle
{
public:
	ImVec2 position;

	UIParticle(ImVec2 startPos);

	void update(float dt);

	void render();

private: 
	GLuint texture;
};