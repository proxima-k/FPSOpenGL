#pragma once

#include "../Game.h"
#include "../shooting/Shooter.h"
#include "../shooting/cards/CosineCard.h"
#include "../shooting/cards/SineCard.h"
#include "../shooting/cards/PlaceholderCard1.h"
#include "../shooting/cards/PlaceholderCard2.h"
#include "../shooting/cards/PlaceholderCard3.h"
#include "../shooting/cards/DamagePassive.h"
#include "../shooting/cards/SpeedPassive.h"
#include "../shooting/cards/DashPassive.h"
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
    void deckShowcase(ImVec2 deckPos, std::queue<Card*> queue, ImVec2 cardPosCenter, ImVec2 cardSize, bool highlightCard = false);
    void randomizeCards();

    GLuint basicCardTexture;
    GLuint sineCardTexture;
    GLuint cosineCardTexture;
    GLuint placeholder1card;
    GLuint placeholder2card;
    GLuint placeholder3card;
    GLuint passivedamagecard;
    GLuint passivespeedcard;
    GLuint passivedashcard;
    GLuint emptydeck;

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