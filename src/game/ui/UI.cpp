#include "UI.h"
#include <stb/stb_image.h>
#include <iostream>
#include <sstream>
#include <string>
#include <random>

#include "../shooting/cards/CosineCard.h"
#include "../shooting/cards/SineCard.h"
#include "../../engine/xyzmath.h"

UI::UI() : crosshair(0), kanitFont(nullptr) {}

UI::~UI() {
    shutdown();
}

void UI::init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    crosshair = loadTextureFromFile("res/sprites/crosshair177.png");

    basicCardTexture = loadTextureFromFile("res/sprites/basiccard.png");
    sineCardTexture = loadTextureFromFile("res/sprites/sinecard.png");
    cosineCardTexture = loadTextureFromFile("res/sprites/cosinecard.png");

    kanitFont = io.Fonts->AddFontFromFileTTF("res/fonts/Kanit-Light.ttf", 60.0f);

    if (!kanitFont) {
        std::cerr << "Failed to load font: Kanit-Light.ttf" << std::endl;
    }
}

void UI::begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::end()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::render(GLFWwindow* window)
{
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 windowSize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("UI Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

    if (kanitFont)
    {
        ImGui::PushFont(kanitFont);
    }

    ImVec2 cardSize(150, 220);
    ImVec2 cardPosCenter((windowSize.x) / 2.0f - (cardSize.x / 2), (windowSize.y) * 0.75f - (cardSize.y / 2));

    switch (game->currentGameState)
    {
    case Game::GameStates::Playing:
            renderPlayModeUI(windowSize);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            cardsRandomized = false;
            highlightCard = false;

            deckShowcase(selectionXSpacing, cardPosCenter, cardSize);
        break;

    case Game::GameStates::SelectCards:
            renderCardSelection(windowSize);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            highlightCard = true;

            deckShowcase(selectionXSpacing, cardPosCenter, cardSize);
        break;

    case Game::GameStates::Dead:
            ImGui::Text("HA DEAD");
        break;
    }

    if (kanitFont)
    {
        ImGui::PopFont();
    }

    ImGui::End();
}

void UI::renderCardSelection(ImVec2 windowSize)
{
    int cardsPerRow = (selectionAmount > 2) ? (selectionAmount + 1) / 2 : selectionAmount;
    int rows = (selectionAmount + cardsPerRow - 1) / cardsPerRow;

    float halfXOffset = (cardsPerRow - 1) / 2.0f;
    float halfYOffset = (rows - 1) / 2.0f;

    ImVec2 cardSize(150, 220);
    ImVec2 cardPosCenter((windowSize.x) / 2.0f - (cardSize.x / 2), (windowSize.y) / 2.0f - (cardSize.y / 2));

    randomizeCards();

    // card states
    static std::vector<float> cardScales(selectionAmount, 1.0f);
    static std::vector<float> cardTimes(selectionAmount, 0.0f);
    static std::vector<bool> cardIsAnimating(selectionAmount, false);

    for (int i = 0; i < selectionAmount; i++)
    {
        int row = i / cardsPerRow;
        int indexInRow = i % cardsPerRow;

        ImVec2 offset = ImVec2((indexInRow - halfXOffset) * selectionXSpacing - centerOffset, (row - halfYOffset) * selectionYSpacing);
        ImVec2 cardPos(cardPosCenter.x + offset.x, cardPosCenter.y + offset.y);

        ImGui::SetCursorPos(cardPos);
        ImGui::PushID(i);

        bool isHovered = ImGui::IsMouseHoveringRect(cardPos, ImVec2(cardPos.x + cardSize.x, cardPos.y + cardSize.y));
        float targetScale = isHovered ? scaleMultiplier : 1.0f;

        if (isHovered && !cardIsAnimating[i]) {
            cardIsAnimating[i] = true;
        }

        // only update the animation if the card is currently scaling
        if (cardIsAnimating[i]) {

            cardTimes[i] += ImGui::GetIO().DeltaTime * scaleSpeed;
            if (cardTimes[i] > 1.0f) cardTimes[i] = 1.0f; 

            // apply the easing function for clear bouncing
            cardScales[i] = 1.0f + (targetScale - 1.0f) * xyzmath::EaseOutElastic(cardTimes[i]);

            // stop the animation if scaling down and finished
            if (!isHovered && cardTimes[i] >= 1.0f) {
                cardIsAnimating[i] = false;
                cardTimes[i] = 0.0f;
            }
        }

        ImVec2 scaledCardSize = ImVec2(cardSize.x * cardScales[i], cardSize.y * cardScales[i]);

        // adjust position based on scaling to keep the card centered
        ImVec2 scaleOffset = ImVec2((scaledCardSize.x - cardSize.x) / 2, (scaledCardSize.y - cardSize.y) / 2);
        ImVec2 adjustedPos = ImVec2(cardPos.x - scaleOffset.x, cardPos.y - scaleOffset.y);

        ImGui::SetCursorPos(adjustedPos);
        bool clicked = ImGui::ImageButton((void*)(intptr_t)selectedTextures[i], scaledCardSize);

        if (clicked)
        {
            std::cout << "Pressed button " << i << std::endl;

            if (selectedTextures[i] == sineCardTexture)
            {
                shooter->cardQueue.push(new SineCard(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera)));
            }
            else if (selectedTextures[i] == cosineCardTexture)
            {
                shooter->cardQueue.push(new CosineCard(glm::vec3(0), MeshRenderer(shooter->cardMesh, shooter->cardShader, shooter->camera)));
            }

            game->currentGameState = Game::GameStates::Playing;

            if (shooter->cardQueue.size() > maxCardHandSize)
            {
                shooter->cardQueue.pop();
            }
        }

        ImGui::PopID();
    }
}




void UI::deckShowcase(float selectionXSpacing, ImVec2 cardPosCenter, ImVec2 cardSize)
{
    if (shooter->cardQueue.size() <= 0) return;

    std::vector<Card*> tempVector;

    std::queue<Card*> tempQueue = shooter->cardQueue;

    float deckYSpacing = 20;

    while (!tempQueue.empty()) {
        tempVector.push_back(tempQueue.front());
        tempQueue.pop();
    }

    for (int i = shooter->cardQueue.size() - 1; i >= 0; i--)
    {
        float deckXOffset((2 - (2 - 1) / 2.0f) * selectionXSpacing);
        float deckYOffset((i - (2 - 1) / 2.0f) * deckYSpacing - (shooter->cardQueue.size() * deckYSpacing / 2));

        if (highlightCard && i == 0) {
            highlightProgress += ImGui::GetIO().DeltaTime * 5;

            if (highlightProgress > 1.0f) highlightProgress = 1.0f;

            highlightCurrentOffset = glm::mix(0.0f, highlightOffsetMax, highlightProgress);
            deckYOffset -= highlightCurrentOffset;
        }
        else if (!highlightCard) {
			highlightProgress = 0.0f;
		}

        ImVec2 cardPos(cardPosCenter.x + deckXOffset, cardPosCenter.y + deckYOffset);

        ImGui::SetCursorPos(cardPos);

        switch (tempVector[i]->getCardType())
        {
        case Card::CardType::Sine:
            ImGui::Image((void*)(intptr_t)sineCardTexture, cardSize);
            break;

        case Card::CardType::Cosine:
            ImGui::Image((void*)(intptr_t)cosineCardTexture, cardSize);
            break;

        default:
            ImGui::Image((void*)(intptr_t)basicCardTexture, cardSize);
            break;
        }
    }
}
void UI::randomizeCards()
{
    std::vector<GLuint> cardTextures = { sineCardTexture, cosineCardTexture };

    if (!cardsRandomized) {
        std::random_device rd;
        std::mt19937 rng(rd());

        selectedTextures.resize(selectionAmount);
        for (int i = 0; i < selectionAmount; i++) {
            std::uniform_int_distribution<int> dist(0, cardTextures.size() - 1);
            selectedTextures[i] = cardTextures[dist(rng)];
        }

        cardsRandomized = true;
    }
}

void UI::renderPlayModeUI(ImVec2 windowSize)
{
    float maxScore = game->maxPlayerXP;
    float playerScore = game->crtPlayerXP;
    float clampedScore = glm::clamp(playerScore, 0.0f, maxScore);
    float crtScoreFraction = clampedScore / maxScore;

    ImVec2 barSize(700, 25);
    ImVec4 barColor(1.00f, 0.91f, 0.32f, 1.0f);

    customProgressBar(crtScoreFraction, barSize, barColor);

    ImVec2 crosshairSize(40, 40);
    ImVec2 crosshairPos((windowSize.x) / 2.0f - (crosshairSize.x / 2), (windowSize.y) / 2.0f - (crosshairSize.y / 2));

    ImGui::SetCursorPos(crosshairPos);
    ImGui::Image((void*)(intptr_t)crosshair, crosshairSize);
}
void UI::customProgressBar(float fraction, const ImVec2& size, const ImVec4& barColor)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    drawList->AddRectFilled(
        pos,
        ImVec2(pos.x + size.x * fraction, pos.y + size.y),
        ImGui::GetColorU32(barColor)
    );
}

void UI::renderGameOverUI(ImVec2 windowSize)
{

}

void UI::shutdown()
{
    if (initialized) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        initialized = false; // Reset flag after shutdown
    }
}

GLuint UI::loadTextureFromFile(const char* filename)
{
    int width, height, channels;
    unsigned char* pixels = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);
    if (!pixels)
    {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);

    return textureID;
}
