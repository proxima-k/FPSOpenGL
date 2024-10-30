#pragma once

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Game.h"
#include "../shooting/Shooter.h"

#include "UICards.h"
#include "UIParticleController.h"

class UI
{
public:
    UI();
    ~UI();

    void init(GLFWwindow* window);
    void update(float dt);
    void render(GLFWwindow* window);
    void begin();
    void end();
    void shutdown();

    void spawn_particle_ctrl();

    void renderPlayModeUI(ImVec2 windowSize);
    void renderGameOverUI(ImVec2 windowSize);

    void customProgressBar(float fraction, const ImVec2& size, const ImVec4& barColor);

    float displayedScoreFraction = 0.0f;

private:
    std::vector<UIParticleController*> particleControllers;

    UICards cards;

    GLuint crosshair;
    ImFont* kanitFont;

    bool initialized;
};
