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

#include "engine/Debug.h"
#include "engine/Logger.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_movement_callback(GLFWwindow* window, double xPos, double yPos);

// window settings
unsigned int windowWidth = 640;
unsigned int windowHeight = 640;

// mouse input handling
float mouseLastX = windowWidth / 2.f;
float mouseLastY = windowHeight / 2.f;

// time between loops
bool firstMouse = true;
float deltaTime = 0.f;
float lastFrameTime = 0.f;

Camera camera(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
Player player(&camera);
Game* game;

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }
    //std::cout << glGetString(GL_VERSION) << std::endl;

    // setup IMGUI context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    bool show_demo_window = true;
    bool show_log_window = true;

    game = new Game();

    Logger logger;
    Debug::setCallback(std::bind(&Logger::onLog, &logger, std::placeholders::_1, std::placeholders::_2));
    
    {
        // Entity (mesh path, shader, camera)
        std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("res/models/teapot.obj");
        Mesh teapotMesh(vertices);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        game->spawn_entity<Entity>(&teapotMesh, &shader, &camera);
        //game->spawn_entity(teapotEntity);

        glEnable(GL_DEPTH_TEST);

        /* Loop until the user closes the window */
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwTerminate();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //ImGui::ShowDemoWindow(&show_demo_window);
            logger.draw("Logger", &show_log_window);

            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrameTime;
            lastFrameTime = currentFrame;


            game->update();
            game->render();

            player.update(window, deltaTime);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

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
    camera.processMouseMovement(static_cast<float>(mouseXPos), static_cast<float>(mouseYPos));
}