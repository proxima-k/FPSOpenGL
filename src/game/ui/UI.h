#pragma once

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Game.h"
#include "../shooting/Shooter.h"
#include "UICards.h"

class UI
{
public:
    UI();
    ~UI();

    void init(GLFWwindow* window);
    void render(GLFWwindow* window);
    void begin();
    void end();
    void shutdown();

    void renderPlayModeUI(ImVec2 windowSize);
    void renderGameOverUI(ImVec2 windowSize);
    void renderBossUI(ImVec2 windowSize);

    void customProgressBar(float fraction, const ImVec2& size, const ImVec4& barColor);

    float displayedScoreFraction = 0.0f;

private:
    void resetCursorPos(GLFWwindow* window);
    bool hasResetCursor = false;
    const float CURSOR_CLICK_COOLDOWN = 0.7f;
    float cursorClickTimer = CURSOR_CLICK_COOLDOWN;

    UICards cards;

    GLuint crosshair;
    GLuint damageScreenEffect;
    ImFont* kanitFont;
    ImFont* menuFont;

    bool initialized;

    float timeElapsed = 0.f;
};
