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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_movement_callback(GLFWwindow* window, double xPos, double yPos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// window settings
unsigned int windowWidth = 700;
unsigned int windowHeight = 700;

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
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_POSITION_X, windowWidth / 5 + windowWidth);
    glfwWindowHint(GLFW_POSITION_Y, windowHeight / 3);

    /* Create a windowed mode window and its OpenGL context */
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
    

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    //std::cout << glGetString(GL_VERSION) << std::endl;

    bool show_demo_window = true;
    bool show_log_window = true;

    game = new Game();
    Camera::mainCamera = &playerCamera;

    Logger logger;
    Debug::setCallback(std::bind(&Logger::onLog, &logger, std::placeholders::_1, std::placeholders::_2));
    
    {
        Grid floorGrid(1, 1, 16);
        Shader gridShader("res/shaders/grid.shader");
        floorGrid.setShader(&gridShader);
        floorGrid.setCamera(&playerCamera);
        floorGrid.setPlayer(&player);

        // Entity (mesh path, shader, camera)
        std::vector<float> enemyCubeVertices = Mesh::getMeshVerticesFromObjFile("res/models/cube.obj");
        Mesh enemyCubeMesh(enemyCubeVertices);
        Shader meshShader("res/shaders/mesh.shader");

        MeshRenderer enemyCubeMeshRenderer(&enemyCubeMesh, &meshShader, Camera::mainCamera);

        std::vector<float> teapotVertices = Mesh::getMeshVerticesFromObjFile("res/models/teapot.obj");
        Mesh teapotMesh(teapotVertices);

        MeshRenderer teapotMeshRenderer(&teapotMesh, &meshShader, Camera::mainCamera);

        // setup post process components 
        float texVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
             1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
             1.0f, -1.0f,  1.0f, 0.0f,
             1.0f,  1.0f,  1.0f, 1.0f
        };
        Shader outlineShader("res/shaders/postprocess.shader");
        VertexArray quadVAO;
        VertexBuffer quadVBO(texVertices, sizeof(texVertices));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        quadVAO.AddBuffer(quadVBO, layout);

        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        unsigned int colorBufferTexture;
        glGenTextures(1, &colorBufferTexture);
        glBindTexture(GL_TEXTURE_2D, colorBufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferTexture, 0);

        unsigned int depthBufferTexture;
        glGenTextures(1, &depthBufferTexture);
        glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferTexture, 0);

        GLuint normalTexture;
        glGenTextures(1, &normalTexture);
        glBindTexture(GL_TEXTURE_2D, normalTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, windowWidth, windowHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normalTexture, 0);

        GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
        glDrawBuffers(2, attachments);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR: FRAMEBUFFER NOT COMPLETE" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);



        // setup card mesh, shader and camera
        player.shooter.setCardRenderer(&enemyCubeMesh, &meshShader, &playerCamera);

		player.shooter.setPlayer(&player);
		player.shooter.setupUI();

        game->setMeshRenderer(&enemyCubeMesh, &meshShader, &playerCamera);

        glEnable(GL_DEPTH_TEST);

        ui.Init(window);

        while (!glfwWindowShouldClose(window))
        {
            // LOGICS =========================================================
            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwTerminate();

            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrameTime;
            lastFrameTime = currentFrame;

            player.update(window, deltaTime);
            game->update();
            floorGrid.update();

            // GRAPHICS =======================================================
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            game->render();
            teapotMeshRenderer.draw(glm::vec3(0, 0.2f, 3), glm::quat(1, 0, 0, 0), glm::vec3(0.008f));
			enemyCubeMeshRenderer.draw(glm::vec3(3, 0.2f, 0), glm::quat(1, 0, 0, 0), glm::vec3(0.5f));
            floorGrid.draw();

            ui.Begin();
            ui.Render();
            ui.End();

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            outlineShader.Bind();
            glDisable(GL_DEPTH_TEST);
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colorBufferTexture);
            glUniform1i(glGetUniformLocation(outlineShader.GetID(), "u_colorTexture"), 0);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
            glUniform1i(glGetUniformLocation(outlineShader.GetID(), "u_depthTexture"), 1);

            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, normalTexture);
            glUniform1i(glGetUniformLocation(outlineShader.GetID(), "u_normalTexture"), 2);

            quadVAO.Bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
    }
    ui.Shutdown();

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
    playerCamera.processMouseMovement(static_cast<float>(mouseXPos), static_cast<float>(mouseYPos));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    player.mouse_button_callback(window, button, action, mods);
}