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

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

#include "graphics/Renderer.h"
#include "graphics/VertexBuffer.h"
#include "graphics/VertexBufferLayout.h"
#include "graphics/IndexBuffer.h"
#include "graphics/VertexArray.h"
#include "graphics/Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_movement_callback(GLFWwindow* window, double xPos, double yPos);
void processInput(GLFWwindow* window);

// window settings
unsigned int windowWidth = 640;
unsigned int windowHeight = 640;

// camera settings
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.0f;
float pitch = 0.0f;

// mouse input handling
float mouseLastX = windowWidth / 2.f;
float mouseLastY = windowHeight / 2.f;

// time between loops
bool firstMouse = true;
float deltaTime = 0.f;
float lastFrameTime = 0.f;

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        std::string inputFile = "res/models/teapot.obj";
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = "./";

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputFile, reader_config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader Error: " << reader.Error();
            }
            exit(1);
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader Warning: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();

        // CREATING A VERTEX BUFFER FILLED WITH VERTEX POSITIONS
        std::vector<float> vertices;
        

        for (const auto& shape : shapes) {
            for (const auto& index : shape.mesh.indices) {
                float vertexXPos = attrib.vertices[3 * index.vertex_index + 0];
                float vertexYPos = attrib.vertices[3 * index.vertex_index + 1];
                float vertexZPos = attrib.vertices[3 * index.vertex_index + 2];

                //std::cout << "X: " << vertexXPos << ", Y: " << vertexYPos << ", Z: " <<  vertexZPos << std::endl;

                vertices.push_back(vertexXPos);
                vertices.push_back(vertexYPos);
                vertices.push_back(vertexZPos);
            }
        }

        std::cout << vertices.size() << std::endl;


        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        VertexArray vao;
        VertexBuffer vbo(&vertices[0], vertices.size() * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3);
        //layout.Push<float>(2);
        vao.AddBuffer(vbo, layout);


        // MATRIX STUFF FOR CAMERA ===============================================================================

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 100.f);

        glEnable(GL_DEPTH_TEST);

        /* Loop until the user closes the window */
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            float currentFrameTime = glfwGetTime();
            deltaTime = currentFrameTime - lastFrameTime;
            lastFrameTime = currentFrameTime;

            processInput(window);

            glClear(GL_COLOR_BUFFER_BIT);
            glClear(GL_DEPTH_BUFFER_BIT);

            glm::mat4 view;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            float height = glm::sin(currentFrameTime * 2) * 0.25f;

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.f, -0.25f + height, 0.f));
            model = glm::rotate(model, glm::radians(currentFrameTime * 100), glm::vec3(0.f, 1.f, 0.f));
            model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));

            shader.Bind();
            unsigned int viewLoc = glGetUniformLocation(shader.GetID(), "u_View");
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            unsigned int projLoc = glGetUniformLocation(shader.GetID(), "u_Projection");
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
            unsigned int modelLoc = glGetUniformLocation(shader.GetID(), "u_Model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            vao.Bind();
            GLCall(glDrawArrays(GL_TRIANGLES, 0, vertices.size()));

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            glfwPollEvents();

        }
    }

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
    float xPos = static_cast<float>(mouseXPos);
    float yPos = static_cast<float>(mouseYPos);

    if (firstMouse)
    {
        mouseLastX = xPos;
        mouseLastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - mouseLastX;
    float yoffset = mouseLastY - yPos; // reversed since y-coordinates go from bottom to top
    mouseLastX = xPos;
    mouseLastY = yPos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void processInput(GLFWwindow* window)
{
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    //std::cout << "X: " << cameraPos.x << " Y:" << cameraPos.y << " Z:" << cameraPos.z << std::endl;
}