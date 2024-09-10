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

    void init(GLFWwindow* window);
    void begin();
    void end();
    void render(GLFWwindow* window);
    void shutdown();

    void renderCardSelection(ImVec2 windowSize);
    void renderPlayModeUI(ImVec2 windowSize);
    void renderGameOverUI(ImVec2 windowSize);

    void customProgressBar(float fraction, const ImVec2& size, const ImVec4& barColor);

    GLuint basicCardTexture;
    GLuint sineCardTexture;
    GLuint cosineCardTexture;

private:
    GLuint loadTextureFromFile(const char* filename);

    GLuint crosshair;
    ImFont* kanitFont;
    bool initialized;
};
