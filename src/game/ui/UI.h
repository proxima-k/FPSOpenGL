#pragma once

#include <glew/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "../Game.h"
#include "../shooting/Shooter.h"

class UI
{
public:
    UI();
    ~UI();

    void Init(GLFWwindow* window);
    void Begin();
    void End();
    void Render();
    void Shutdown();

    void RenderCardSelection(ImVec2 windowSize);
    void RenderPlayModeUI(ImVec2 windowSize);
    void RenderGameOverUI(ImVec2 windowSize);

    void CustomProgressBar(float fraction, const ImVec2& size, const ImVec4& barColor);

    GLuint basicCardTexture;

private:
    GLuint LoadTextureFromFile(const char* filename);

    GLuint crosshair;
    ImFont* kanitFont;
};
