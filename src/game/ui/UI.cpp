#include "UI.h"
#include <stb/stb_image.h>
#include <iostream>
#include <sstream>
#include <string>

GLuint UI::crosshair = 0;
ImFont* UI::kanitFont = nullptr;

void UI::Init(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    crosshair = LoadTextureFromFile("res/sprites/crosshair177.png");

    kanitFont = io.Fonts->AddFontFromFileTTF("res/fonts/Kanit-Light.ttf", 60.0f);
    if (!kanitFont) {
        std::cerr << "Failed to load font: Kanit-Light.ttf" << std::endl;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void UI::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void UI::End()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::Render()
{
    ImGuiIO& io = ImGui::GetIO();

    ImVec2 windowSize = io.DisplaySize;

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("Image Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
    
    if (kanitFont) 
    {
        ImGui::PushFont(kanitFont); // use the loaded font
    }

    // draw current score

    ImVec2 scoreposition(0, 0);

    int score = 0;
   
    std::stringstream ss;
    ss << "Score: " << game->playerScore;
    std::string scoreStr = ss.str();
    const char* scoreText = scoreStr.c_str();
    
    ImGui::SetCursorPos(scoreposition);

    float maxScore = 100;
    float playerScore = game->playerScore;
    float clampedScore = glm::clamp(playerScore, 0.0f, maxScore);
    float crtScoreFraction = clampedScore / maxScore;

    ImVec2 barSize(700, 25);
    ImVec4 barColor(1.00f, 0.91f, 0.32f, 1.0f);

    CustomProgressBar(crtScoreFraction, barSize, barColor);

    if (kanitFont) 
    {
        ImGui::PopFont(); // revert to the default font
    }

    // draw crosshair
    ImVec2 crosshairSize(40, 40);
    ImVec2 crosshairPos((windowSize.x) / 2.0f - (crosshairSize.x / 2), (windowSize.y) / 2.0f - (crosshairSize.y / 2));

    ImGui::SetCursorPos(crosshairPos);
    ImGui::Image((void*)(intptr_t)crosshair, crosshairSize);

    ImGui::End();
}

void UI::CustomProgressBar(float fraction, const ImVec2& size, const ImVec4& barColor)
{
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    ImVec2 pos = ImGui::GetCursorScreenPos();

    // progress
    drawList->AddRectFilled(
        pos,
        ImVec2(pos.x + size.x * fraction, pos.y + size.y),
        ImGui::GetColorU32(barColor)
    );
}

void UI::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

GLuint UI::LoadTextureFromFile(const char* filename)
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
