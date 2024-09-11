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
    void render(GLFWwindow* window);
    void begin();
    void end();
    void shutdown();

    void renderCardSelection(ImVec2 windowSize);
    void deckShowcase(float selectionXSpacing, ImVec2 cardPosCenter, ImVec2 cardSize);
    void randomizeCards();
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

    int selectionAmount = 2;
    int maxCardHandSize = 3;

    float selectionXSpacing = 170.0f;
    float selectionYSpacing = 250.0f;
    float centerOffset = 100;
    float scaleMultiplier = 1.2f;
    float scaleSpeed = 2.0f;

    float highlightOffsetMax = 70.0f;
    float highlightOffsetMin = 0.0f;
    float highlightCurrentOffset = 0.0f;
    float highlightProgress = 0.0f;

    bool cardsRandomized = false;
    bool highlightCard = false;
    bool initialized;

    std::vector<GLuint> selectedTextures;

};
