#include <glew/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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
#include "graphics/VertexBuffer.h"
#include "graphics/VertexBufferLayout.h"
#include "graphics/IndexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"

#include "graphics/Mesh.h"

#include "game/Camera.h"
#include "game/Player.h"
#include "game/Game.h"
#include "game/Entity.h"
#include "game/enemies/CubeEnemy.h"

#include "game/shooting/Card.h"

#include "engine/Debug.h"
#include "engine/Logger.h"

#include "game/Grid.h"

#include "game/ui/UI.h"

#include "game/Spawner.h"

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

Camera playerCamera(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), windowWidth, windowHeight);
Player player(&playerCamera);
UI ui;
Game* game = nullptr;

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

    bool show_demo_window = true;
    bool show_log_window = true;

    game = new Game();
    Camera::mainCamera = &playerCamera;



    /*Logger logger;
    Debug::setCallback(std::bind(&Logger::onLog, &logger, std::placeholders::_1, std::placeholders::_2));*/
    
    {
        Grid floorGrid(1, 1, 16);
        Shader gridShader("res/shaders/grid.shader");
        floorGrid.setShader(&gridShader);
        floorGrid.setCamera(&playerCamera);
        floorGrid.setPlayer(&player);

        // Entity (mesh path, shader, camera)
        std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("res/models/cube.obj");
        Mesh cubeMesh(vertices);

        Shader meshShader("res/shaders/Basic.shader");

        MeshRenderer cubeMeshRenderer(&cubeMesh, &meshShader, Camera::mainCamera);

        // setup card mesh, shader and camera
        player.shooter.setCardRenderer(&cubeMesh, &meshShader, &playerCamera);

		player.shooter.setPlayer(&player);

        game->setMeshRenderer(&cubeMesh, &meshShader, &playerCamera);

        glEnable(GL_DEPTH_TEST);

        ui.init(window);

		Spawner<CubeEnemy> cubeEnemySpawner(1.f, cubeMeshRenderer, &player);

        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwTerminate();

            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrameTime;
            lastFrameTime = currentFrame;

            if (game->currentGameState == Game::GameStates::Playing)
            {
                player.update(window, deltaTime);
            }

            game->update();
            game->render();

			cubeEnemySpawner.update(deltaTime);

            floorGrid.update();
            floorGrid.draw();

            ui.begin();
            ui.render(window);
            ui.end();

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
    glViewport(0, 0, width, height);
    windowWidth = width;
    windowHeight = height;
}

void mouse_movement_callback(GLFWwindow* window, double mouseXPos, double mouseYPos)
{
    if (game->currentGameState == Game::GameStates::Playing)
    playerCamera.processMouseMovement(static_cast<float>(mouseXPos), static_cast<float>(mouseYPos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (game->currentGameState == Game::GameStates::Playing)
    player.mouse_button_callback(window, button, action, mods);
}