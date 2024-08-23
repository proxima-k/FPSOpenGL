#include "UI.h"
#include <stb/stb_image.h>
#include <iostream>

GLuint UI::detectioncircle = 0;
GLuint UI::crosshair = 0;

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

    detectioncircle = LoadTextureFromFile("res/sprites/detectioncircle.png");
    crosshair = LoadTextureFromFile("res/sprites/crosshair177.png");
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
    if (detectioncircle == 0 || crosshair == 0)
    {
		std::cerr << "Textures not loaded" << std::endl;
		return;
	}

    ImGuiIO& io = ImGui::GetIO();

    ImVec2 windowSize = io.DisplaySize;


    // draw detection circle
    ImVec2 detectionCircleSize(200, 200);
    ImVec2 detectionCirclePosition((windowSize.x) / 2.0f - (detectionCircleSize.x / 2), (windowSize.y) / 2.0f - (detectionCircleSize.y / 2));

    static float rotationAngle = 0.0f;
    rotationAngle += 0.01f;

    // calculate sine and cosine of the angle
    float cosAngle = cos(rotationAngle);
    float sinAngle = sin(rotationAngle);

    // calculate the rotated corner positions
    ImVec2 vertices[4];
    vertices[0] = ImVec2(-detectionCircleSize.x / 2.0f, -detectionCircleSize.y / 2.0f);
    vertices[1] = ImVec2(detectionCircleSize.x / 2.0f, -detectionCircleSize.y / 2.0f);
    vertices[2] = ImVec2(detectionCircleSize.x / 2.0f, detectionCircleSize.y / 2.0f);
    vertices[3] = ImVec2(-detectionCircleSize.x / 2.0f, detectionCircleSize.y / 2.0f);

    for (int i = 0; i < 4; ++i)
    {
        float x = vertices[i].x;
        float y = vertices[i].y;
        vertices[i].x = cosAngle * x - sinAngle * y + (windowSize.x / 2);
        vertices[i].y = sinAngle * x + cosAngle * y + (windowSize.y / 2);
    }

    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(windowSize);

    ImGui::Begin("Image Window", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddImageQuad(
        (void*)(intptr_t)detectioncircle,
        vertices[0],
        vertices[1],
        vertices[2],
        vertices[3]
    );

    // draw crosshair
    ImVec2 crosshairSize(40, 40);
    ImVec2 crosshairPos((windowSize.x) / 2.0f - (crosshairSize.x / 2), (windowSize.y) / 2.0f - (crosshairSize.y / 2));

    ImGui::SetCursorPos(crosshairPos);
    ImGui::Image((void*)(intptr_t)crosshair, crosshairSize);

    ImGui::End();
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
