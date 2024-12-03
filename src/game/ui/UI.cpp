#include "UI.h"
#include <stb/stb_image.h>
#include <iostream>
#include <sstream>
#include <string>
#include <random>

#include "../Player.h"

#include "../shooting/cards/CosineCard.h"
#include "../shooting/cards/SineCard.h"

#include "../enemies/boss/BossEnemy.h"

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

    GLuint(*getTexture)(std::string) = [](std::string fileName) -> GLuint {
        return game->textureManager->getTexture(fileName);
        };

    damageScreenEffect = getTexture("damage");
    crosshair = getTexture("crosshair177");

    cards.basicCardTexture = getTexture("basiccard");
    cards.sineCardTexture = getTexture("sinecard");
    cards.cosineCardTexture = getTexture("cosinecard");
    cards.placeholder1card = getTexture("x3");
    cards.placeholder2card = getTexture("pull");
    cards.placeholder3card = getTexture("circle");
    cards.passivedamagecard = getTexture("damagebuffcard");
    cards.passivespeedcard = getTexture("speedbuffcard");
    cards.passivedashcard = getTexture("dashbuffcard");
    cards.emptydeck = getTexture("emptydeck");

    kanitFont = io.Fonts->AddFontFromFileTTF("res/fonts/SpaceMono-Bold.ttf", 40.0f);
    menuFont = io.Fonts->AddFontFromFileTTF("res/fonts/SpaceMono-Bold.ttf", 120.0f);

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

    //std::cout << game->getCurrentState() << std::endl;
    switch (game->getCurrentState())
    {
    case GameStateManager::State::BossFight:
            renderBossUI(windowSize);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            cards.cardsRandomized = false;
            cards.highlightCard = false;

            cards.deckShowcase(deckPosPassives, shooter->cardPassivesQueue, cardPosCenter, cardSize);
            cards.deckShowcase(deckPosActives, shooter->cardQueue, cardPosCenter, cardSize);
        break;

    case GameStateManager::State::Playing:
            renderPlayModeUI(windowSize);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            cards.cardsRandomized = false;
            cards.highlightCard = false;

            cards.deckShowcase(deckPosPassives, shooter->cardPassivesQueue, cardPosCenter, cardSize);
            cards.deckShowcase(deckPosActives, shooter->cardQueue, cardPosCenter, cardSize);
        break;

    case GameStateManager::State::SelectCards:
            renderPlayModeUI(windowSize);
            cards.renderCardSelection(windowSize);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            cards.highlightCard = true;

            cards.deckShowcase(deckPosPassives, shooter->cardPassivesQueue, cardPosCenter, cardSize);
            cards.deckShowcase(deckPosActives, shooter->cardQueue, cardPosCenter, cardSize);
        break;

    case GameStateManager::State::Dead:
            ImGui::Text("HA DEAD");
        break;

    case GameStateManager::State::MainMenu:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            ImVec2 menuSpriteSize = { windowWidth, windowHeight };
            ImGui::Image((void*)(intptr_t)game->textureManager->getTexture("menusprite"), menuSpriteSize);

            // PLAY BUTTON
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.f, 1.f, 1.f, 1.f));          // Button idle color
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.85f, 0.85f, 0.85f, 1.f));   // Button hover color
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.6f, 0.6f, 1.f));    // Button active color
            float buttonSizeX = (400);
            float buttonSizeY = (150);
            ImGui::SetCursorPos({ (windowWidth / 7.5f) - (buttonSizeX / 2), (windowHeight / 2) - (buttonSizeY / 2) });
            ImVec2 buttonSize = { buttonSizeX , buttonSizeY };
            //ImGui::PushStyleColor(ImGuiCol_Button, { 2, 2, 2, 2 });
            bool clicked = ImGui::ImageButton((void*)(intptr_t)game->textureManager->getTexture("play"), buttonSize);
            if (clicked) {
                game->reset();
                game->changeState(GameStateManager::State::Playing);
            }
            ImGui::PopStyleColor(3);

            /*if (menuFont)
            {
                ImGui::PopFont();
                ImGui::PushFont(menuFont);
            }*/

            // TITLE
            float titleSizeX = (500);
            //float titleSizeX = (windowWidth / 2.f);
            //float titleSizeY = (windowHeight / 2.f);
            float titleSizeY = (500);
            ImVec2 titleSize = { titleSizeX , titleSizeY };

            ImGui::SetCursorPos({ (windowWidth / 7.5f) - (titleSizeX / 2), (windowHeight / 4.f) - (titleSizeY / 2) });
            ImGui::Image((void*)(intptr_t)game->textureManager->getTexture("(xyz)^0"), (titleSize));



            // CHEATSHEET
            float cheatSizeX = (500);
            float cheatSizeY = (250);
            ImVec2 cheatSize = { cheatSizeX , cheatSizeY };

            ImGui::SetCursorPos({ (windowWidth / 6.5f) - (cheatSizeX / 2), (windowHeight * 3.15f / 4.f) - (cheatSizeY / 2)});
            ImGui::Image((void*)(intptr_t)game->textureManager->getTexture("cheatsheet"), (cheatSize));
        break;
    }

    auto player = game->player;
    if (!player->bIsShieldAlive) {
        ImVec2 screenSize = { windowWidth, windowHeight };

        float sCooldown = player->shieldCooldown;
        float sCooldownTimer = player->shieldCooldownTimer;
        sCooldown /= sCooldown;
        sCooldownTimer /= sCooldown;

        float tint = sCooldownTimer - sCooldown;

        ImVec4 tintColor = ImVec4(1.0f, 1.0f, 1.0f, tint);

        ImGui::SetCursorPos({ 0, 0 });
        ImGui::Image((void*)(intptr_t)game->textureManager->getTexture("damage"), screenSize, ImVec2(0, 0), ImVec2(1, 1), tintColor);
    }

    if (kanitFont)
    {
        ImGui::PopFont();
    }

    ImGui::End();
}

void UI::renderPlayModeUI(ImVec2 windowSize)
{
    float maxScore = game->getPlayerMaxXP();
    float playerScore = game->getPlayerXP();
    float clampedScore = glm::clamp(playerScore, 0.0f, maxScore);
    float crtScoreFraction = clampedScore / maxScore;
    float lerpSpeed = 0.1f;
    
    static float currentCrosshairSize = 40.0f;
    static float targetCrosshairSize = 40.0f;
    const float lerpSpeedCrosshair = 0.2f;
    const float enlargedSize = 55.0f;
    
    float windowWidth = ImGui::GetWindowWidth();
    
    ImVec2 levelPos((windowSize.x) / 2.0f - (40 / 2), 0);
    ImVec2 clockPos((windowSize.x) / 2.0f - (40 / 2), windowSize.y - 40);
    ImVec2 crosshairPos((windowSize.x) / 2.0f - (currentCrosshairSize / 2), (windowSize.y) / 2.0f - (currentCrosshairSize / 2));
    ImVec2 barSize(windowWidth, 30);
    ImVec4 barColor(0.447f, 0.294f, 1.f, 1.0f);
    
    int minutes = static_cast<int>(game->timeLeftUntilBoss) / 60;
    int seconds = static_cast<int>(game->timeLeftUntilBoss) % 60;

    customProgressBar(displayedScoreFraction, barSize, barColor);

    char timeText[16];
    std::snprintf(timeText, sizeof(timeText), "%02d:%02d", minutes, seconds);

    ImGui::SetCursorPos(clockPos);
    ImGui::Text("%s", timeText);

    displayedScoreFraction += (crtScoreFraction - displayedScoreFraction) * lerpSpeed;
    
    ImGui::SetCursorPos(levelPos);
    ImGui::Text("LVL %d", game->get_player_level());
    
    if (ImGui::IsMouseClicked(0))
    {
        targetCrosshairSize = enlargedSize;
    }
    currentCrosshairSize += (targetCrosshairSize - currentCrosshairSize) * lerpSpeedCrosshair;
    
    if (currentCrosshairSize >= enlargedSize - 5)
    {
        targetCrosshairSize = 40.0f;
    }
    
    ImGui::SetCursorPos(crosshairPos);
    ImGui::Image((void*)(intptr_t)crosshair, ImVec2(currentCrosshairSize, currentCrosshairSize));
}

void UI::customProgressBar(float fraction, const ImVec2& size, const ImVec4& barColor)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    drawList->AddRectFilled(
        pos,
        ImVec2(pos.x + size.x * fraction, pos.y + size.y),
        ImGui::GetColorU32(barColor));
}

void UI::renderGameOverUI(ImVec2 windowSize)
{

}

void UI::renderBossUI(ImVec2 windowSize) {
    BossEnemy* boss = game->bossFightController->getBoss();
    if (boss == nullptr) return;

    auto [crtHP, maxHP] = boss->getTotalCrtHP();
    float displayHpFraction = (maxHP > 0) ? crtHP / maxHP : 0.0f;

    static float previousHpFraction = displayHpFraction;

    const float lerpSpeed = 0.1f;
    previousHpFraction += (displayHpFraction - previousHpFraction) * lerpSpeed;

    ImVec2 barSize(windowSize.x, 30);
    ImVec4 barColor(1.00f, 0.286f, 0.286f, 1.0f);
    customProgressBar(previousHpFraction, barSize, barColor);

    ImVec2 bossName((windowSize.x) / 2.0f - (40 / 2), 0);

    ImGui::SetCursorPos(bossName);
    ImGui::Text("TWO CUBED");

    static float currentCrosshairSize = 40.0f;
    static float targetCrosshairSize = 40.0f;
    const float lerpSpeedCrosshair = 0.2f;
    const float enlargedSize = 55.0f;

    ImVec2 crosshairPos((windowSize.x) / 2.0f - (currentCrosshairSize / 2), (windowSize.y) / 2.0f - (currentCrosshairSize / 2));

    if (ImGui::IsMouseClicked(0))
    {
        targetCrosshairSize = enlargedSize;
    }
    currentCrosshairSize += (targetCrosshairSize - currentCrosshairSize) * lerpSpeedCrosshair;

    if (currentCrosshairSize >= enlargedSize - 5)
    {
        targetCrosshairSize = 40.0f;
    }

    ImGui::SetCursorPos(crosshairPos);
    ImGui::Image((void*)(intptr_t)crosshair, ImVec2(currentCrosshairSize, currentCrosshairSize));
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