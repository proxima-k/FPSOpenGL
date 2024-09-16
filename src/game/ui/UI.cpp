#include "UI.h"
#include <stb/stb_image.h>
#include <iostream>
#include <sstream>
#include <string>
#include <random>

#include "../shooting/cards/CosineCard.h"
#include "../shooting/cards/SineCard.h"

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

    cards.basicCardTexture = loadTextureFromFile("res/sprites/basiccard.png");
    cards.sineCardTexture = loadTextureFromFile("res/sprites/sinecard.png");
    cards.cosineCardTexture = loadTextureFromFile("res/sprites/cosinecard.png");
    cards.placeholder1card = loadTextureFromFile("res/sprites/placeholder1card.png");
    cards.placeholder2card = loadTextureFromFile("res/sprites/placeholder2card.png");
    cards.placeholder3card = loadTextureFromFile("res/sprites/placeholder3card.png");
    cards.emptydeck = loadTextureFromFile("res/sprites/emptydeck.png");

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

    float windowWidth = ImGui::GetWindowWidth();
    float windowHeight = ImGui::GetWindowHeight();

    ImVec2 deckPosPassives(windowWidth - cardSize.x - 20, windowHeight - cardSize.y - 20);
    ImVec2 deckPosActives(windowWidth - cardSize.x * 2 - 40, windowHeight - cardSize.y - 20);

    switch (game->currentGameState)
    {
    case Game::GameStates::Playing:
            renderPlayModeUI(windowSize);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            cards.cardsRandomized = false;
            cards.highlightCard = false;

            // take pos into account and make seperate cardQueue
            cards.deckShowcase(deckPosPassives, shooter->cardPassivesQueue, cardPosCenter, cardSize);
            cards.deckShowcase(deckPosActives, shooter->cardQueue, cardPosCenter, cardSize);
        break;

    case Game::GameStates::SelectCards:
            cards.renderCardSelection(windowSize);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            cards.highlightCard = true;

            // take pos into account and make seperate cardQueue
            cards.deckShowcase(deckPosPassives, shooter->cardPassivesQueue, cardPosCenter, cardSize);
            cards.deckShowcase(deckPosActives, shooter->cardQueue, cardPosCenter, cardSize);
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

void UI::renderPlayModeUI(ImVec2 windowSize)
{
    float maxScore = game->maxPlayerXP;
    float playerScore = game->crtPlayerXP;
    float clampedScore = glm::clamp(playerScore, 0.0f, maxScore);
    float crtScoreFraction = clampedScore / maxScore;

    float windowWidth = ImGui::GetWindowWidth();

    ImVec2 barSize(windowWidth, 25);
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
