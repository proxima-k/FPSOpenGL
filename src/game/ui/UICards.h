#pragma once

#include "../Game.h"
#include "../shooting/Shooter.h"
#include "../shooting/cards/CosineCard.h"
#include "../shooting/cards/SineCard.h"
#include "../../engine/xyzmath.h"

#include <glew/glew.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <vector>
#include <random>

class UICards
{
public:
    void renderCardSelection(ImVec2 windowSize);
    void deckShowcase(ImVec2 cardPosCenter, ImVec2 cardSize);
    void randomizeCards();

    GLuint basicCardTexture;
    GLuint sineCardTexture;
    GLuint cosineCardTexture;

    std::vector<GLuint> selectedTextures;

    int selectionAmount = 2;
    int maxCardHandSize = 3;

    float selectionXSpacing = 170.0f;
    float selectionYSpacing = 250.0f;
    float scaleMultiplier = 1.2f;
    float scaleSpeed = 2.0f;

    float highlightOffsetMax = 70.0f;
    float highlightOffsetMin = 0.0f;
    float highlightCurrentOffset = 0.0f;
    float highlightProgress = 0.0f;

    bool cardsRandomized = false;
    bool highlightCard = false;
};