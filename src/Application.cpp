#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "graphics/Renderer.h"
#include "graphics/VertexArray.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexBufferLayout.h"
#include "graphics/Shader.h"
#include "graphics/Mesh.h"
#include "graphics/FrameBuffer.h"
#include "graphics/OutlinePostProcess.h"

#include "game/Camera.h"
#include "game/Player.h"
#include "game/Game.h"
#include "game/Entity.h"
#include "game/enemies/CubeEnemy.h"
#include "game/enemies/ShootingEnemy.h"

#include "game/Grid.h"
#include "game/WallGrid.h"
#include "game/enemies/boss/BossCage.h"
#include "game/BossFightController.h"

#include "game/ui/UI.h"

#include "game/Spawner.h"

#include "game/AudioManager.h"
#include "engine/SteamManager.h"
#include "engine/MeshManager.h"
#include "engine/ShaderManager.h"

#include "game/enemies/boss/BossEnemy.h"
#include "game/enemies/boss/HealingLine.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_movement_callback(GLFWwindow* window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// window settings
unsigned int windowWidth;
unsigned int windowHeight;

// mouse input handling
float mouseLastX = windowWidth / 2.f;
float mouseLastY = windowHeight / 2.f;

// time between loops
bool firstMouse = true;
float deltaTime = 0.f;
float lastFrameTime = 0.f;

Camera playerCamera(glm::vec3(0.0f, 1.0f, 0.0f), windowWidth, windowHeight);
Player* player = nullptr;
UI ui;
Game* game = nullptr;
Grid* floorGrid = nullptr;
AudioManager* audioManager = nullptr;

//SteamManager* steamManager = nullptr;
/*
GLuint LoadTextureFromFile(const char* filename)
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
*/

int main(void)
{
    if (!glfwInit())
        return -1;

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

    windowWidth = mode->width;
    windowHeight = mode->height;

    playerCamera.updateScreenSize(windowWidth, windowHeight);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "(xyz)^0", NULL, NULL);

    int width, height;
    int channels;
    unsigned char* pixels = stbi_load("C:/Projects/FPSOpenGL/res/sprites/placeholder.jpg", &width, &height, &channels, 4);

    if (pixels == NULL)
    {
        fprintf(stderr, "Failed to load image\n");
    }
    else
    {
        GLFWimage images[1];
        images[0].width = width;
        images[0].height = height;
        images[0].pixels = pixels;

        glfwSetWindowIcon(window, 1, images);

        // Free the image data
        stbi_image_free(pixels);
    }

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // set callbacks
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_movement_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    //std::cout << glGetString(GL_VERSION) << std::endl;

    game = new Game();
    audioManager = new AudioManager();
    //steamManager = new SteamManager();
    Camera::mainCamera = &playerCamera;

    {
        meshManager = new MeshManager();
        shaderManager = new ShaderManager();
		meshManager->init();
        shaderManager->init();


        // setup post processing components
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, windowWidth, windowHeight);
        FrameBuffer FBO(windowWidth, windowHeight);
        OutlinePostProcess outlinePP;
        outlinePP.setColorTextureID(FBO.GetColorID());
        outlinePP.setDepthTextureID(FBO.GetDepthID());
        outlinePP.setNormalTextureID(FBO.GetNormalID());

        // setup card mesh, shader and camera
        /*Shader* meshShader = shaderManager->getShader("mesh");
		Mesh* cubeMesh = meshManager->getMesh("cube");*/

        player = new Player(&playerCamera, window);
        //player->transform.position = glm::vec3(0, 0, 0);
		player->shooter.setPlayer(player);
        //player->shooter.setCardRenderer(cubeMesh, meshShader, &playerCamera);
        //game->setMeshRenderer(cubeMesh, meshShader, &playerCamera);
        game->player = player;
        game->camera = &playerCamera;

        audioManager->init();
        ui.init(window);

        floorGrid = new Grid(16, 16, 1);
        floorGrid->setShader(shaderManager->getShader("grid"));
        floorGrid->setCamera(&playerCamera);
        floorGrid->setPlayer(player);

        game->baseFloorGrid = floorGrid;
        game->init();

        while (!glfwWindowShouldClose(window))
        {
            // LOGICS =========================================================
            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwTerminate();

            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrameTime;
            lastFrameTime = currentFrame;

            /*if (game->currentGameState == Game::GameStates::Playing)
            {
                player->update(deltaTime);
            }*/
            game->update();
            game->render();


            // GRAPHICS =======================================================
            FBO.Bind();
            GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            
            // geometry pass
            game->render();

            // This makes sure after post processing, the depth data is still there for the following meshes to use it 
            FrameBuffer::CopyDepthBuffer(FBO.GetID(), 0, windowWidth, windowHeight);

            // post processing pass
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);
            outlinePP.render();
            
            // grid pass
            game->renderGrid();
            /*bossFightController.drawBossCage();
            if (!bossFightController.bossFightIsActive())
                floorGrid->draw();*/

            // healing lines pass
            game->renderHealingLines();
            
            // UI pass
            ui.begin();
            ui.render(window);
            ui.end();

            // audio
            //audioManager->update();

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
    }
    ui.shutdown();

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

    // todo: potential issues with custom framebuffer
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void mouse_movement_callback(GLFWwindow* window, double mouseXPos, double mouseYPos)
{
    if (game->isInGame())
    //playerCamera.processMouseMovement(static_cast<float>(mouseXPos), static_cast<float>(mouseYPos));
    player->mouse_movement_callback(static_cast<float>(mouseXPos), static_cast<float>(mouseYPos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (game->isInGame())
    player->mouse_button_callback(window, button, action, mods);
}