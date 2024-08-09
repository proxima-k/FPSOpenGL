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

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        std::string inputFile = "res/models/teapot.obj";
        tinyobj::ObjReaderConfig reader_config;
        //reader_config.mtl_search_path

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputFile, reader_config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            exit(1);
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();

        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            std::cout << s << std::endl;
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                    tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
                    tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
                    
                    std::cout << "x: " << vx << ", y: " << vy << ", z: " << vz << std::endl;

                    // Check if `normal_index` is zero or positive. negative = no normal data
                    if (idx.normal_index >= 0) {
                        tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
                        tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
                        tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
                    }

                    // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                    if (idx.texcoord_index >= 0) {
                        tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
                        tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
                    }

                    // Optional: vertex colors
                    // tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
                    // tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
                    // tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
                }
                index_offset += fv;

                // per-face material
                shapes[s].mesh.material_ids[f];
            }
        }

        /* Loop until the user closes the window */
        GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
        while (!glfwWindowShouldClose(window))
        {
            /* Poll for and process events */
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT);

            /*vao.Bind();
            renderer.Draw(vao, ib, shader);*/

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

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
}


//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
//
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <sstream>
//
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//
//#include "Renderer.h"
//#include "VertexBuffer.h"
//#include "VertexBufferLayout.h"
//#include "IndexBuffer.h"
//#include "VertexArray.h"
//#include "Shader.h"
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void mouse_movement_callback(GLFWwindow* window, double xPos, double yPos);
//void processInput(GLFWwindow* window);
//
//// window settings
//unsigned int windowWidth  = 640;
//unsigned int windowHeight = 640;
//
//// camera settings
//glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);
//
//float yaw   = -90.0f;
//float pitch = 0.0f;
//
//// mouse input handling
//float mouseLastX = windowWidth  / 2.f;
//float mouseLastY = windowHeight / 2.f;
//
//// time between loops
//bool firstMouse = true;
//float deltaTime = 0.f;
//float lastFrameTime = 0.f;
//
//int main(void)
//{
//    /* Initialize the library */
//    if (!glfwInit())
//        return -1;
//
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    /* Create a windowed mode window and its OpenGL context */
//    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Hello World", NULL, NULL);
//    if (!window)
//    {
//        glfwTerminate();
//        return -1;
//    }
//
//    /* Make the window's context current */
//    glfwMakeContextCurrent(window);
//    glfwSwapInterval(1);
//
//    // set callbacks
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//    glfwSetCursorPosCallback(window, mouse_movement_callback);
//
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//
//    if (glewInit() != GLEW_OK) {
//        std::cout << "Error!" << std::endl;
//    }
//
//    std::cout << glGetString(GL_VERSION) << std::endl;
//    {
//        float vertices[] = {
//            // positions          // colors           // texture coords
//             0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//             0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
//        };
//        unsigned int indices[] = {
//            0, 1, 2,
//            0, 2, 3,
//        };
//
//        float vertices2[] = {
//            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//             0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//             0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//
//            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//             0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//             0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//             0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//             0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//             0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//             0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
//        };
//
//        glm::vec3 cubePositions[] = {
//            glm::vec3(0.0f,  0.0f,  0.0f),
//            glm::vec3(2.0f,  5.0f, -15.0f),
//            glm::vec3(-1.5f, -2.2f, -2.5f),
//            glm::vec3(-3.8f, -2.0f, -12.3f),
//            glm::vec3(2.4f, -0.4f, -3.5f),
//            glm::vec3(-1.7f,  3.0f, -7.5f),
//            glm::vec3(1.3f, -2.0f, -2.5f),
//            glm::vec3(1.5f,  2.0f, -2.5f),
//            glm::vec3(1.5f,  0.2f, -1.5f),
//            glm::vec3(-1.3f,  1.0f, -1.5f)
//        };
//
//        VertexArray vao;
//        // allocating a buffer in GPU and giving it data
//        VertexBuffer vbo(vertices, sizeof(vertices));
//
//        /*VertexBufferLayout layout;
//        layout.Push<float>(3);
//        layout.Push<float>(3);
//        layout.Push<float>(2);
//        vao.AddBuffer(vbo, layout);
//
//        IndexBuffer ib(indices, 6);*/
//
//        Shader shader("res/shaders/Basic.shader");
//        shader.Bind();
//
//        /*vao.Unbind();
//        vbo.Unbind();
//        ib.Unbind();*/
//        shader.Unbind();
//
//        VertexArray vao2;
//        VertexBuffer vbo2(vertices2, sizeof(vertices2));
//
//        VertexBufferLayout layout2;
//        layout2.Push<float>(3);
//        layout2.Push<float>(2);
//        vao2.AddBuffer(vbo2, layout2);
//
//        Renderer renderer;
//
//        // TEXTURE
//        unsigned int texture;
//        glGenTextures(1, &texture);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        // set the texture wrapping/filtering options (on the currently bound texture object)
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        // load and generate the texture
//        int width, height, nrChannels;
//        unsigned char* data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
//        if (data)
//        {
//            GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
//            glGenerateMipmap(GL_TEXTURE_2D);
//        }
//        else
//        {
//            std::cout << "Failed to load texture" << std::endl;
//        }
//        stbi_image_free(data);
//
//        // MATRIX STUFF
//
//        glm::mat4 view = glm::mat4(1.0f);
//        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
//        view = glm::lookAt(glm::vec3(0.f, 0.f, 3.f),
//                           glm::vec3(0.f, 0.f, 1.f),
//                           glm::vec3(0.f, 1.f, 0.f));
//
//        glm::mat4 projection;
//        projection = glm::perspective(glm::radians(45.0f), 1.f, 0.1f, 100.f);
//
//        glEnable(GL_DEPTH_TEST);
//
//        /* Loop until the user closes the window */
//        GLCall(glClearColor(.1f, .1f, .1f, 1.0f));
//        while (!glfwWindowShouldClose(window))
//        {
//            float currentFrameTime = glfwGetTime();
//            deltaTime = currentFrameTime - lastFrameTime;
//            lastFrameTime = currentFrameTime;
//
//            processInput(window);
//            
//            /* Render here */
//            renderer.Clear();
//            glClear(GL_DEPTH_BUFFER_BIT);
//            
//            /*glm::mat4 model = glm::mat4(1.0f);
//            model = glm::rotate(model, glm::radians((float)glfwGetTime() * 100), glm::vec3(0.65f, 0.5f, 0.8f));*/
//            glm::mat4 view;
//            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
//
//            projection = glm::perspective(glm::radians(45.f), (float)(windowWidth / windowHeight), 0.1f, 100.f);
//
//            /*float time = glfwGetTime();
//            float sin = (sinf(time) / 2.0f + 0.5f);
//            std::cout << sin << std::endl;*/
//
//            GLCall(glBindTexture(GL_TEXTURE_2D, texture));
//            shader.Bind();
//            
//            /*unsigned int modelLoc = glGetUniformLocation(shader.GetID(), "u_Model");
//            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));*/
//            unsigned int viewLoc = glGetUniformLocation(shader.GetID(), "u_View");
//            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//            unsigned int projLoc = glGetUniformLocation(shader.GetID(), "u_Projection");
//            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//            vao2.Bind();
//            for (unsigned int i = 0; i < 10; i++) {
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, cubePositions[i]);
//
//                float angle = 20.0f * i;
//                //model = glm::rotate(model, glm::radians((float)glfwGetTime() * 100 + angle), glm::vec3(0.65f, i / 10.f, 0.8f));
//
//                unsigned int modelLoc = glGetUniformLocation(shader.GetID(), "u_Model");
//                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//                        
//                GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
//            }
//
//
//            /*vao.Bind();
//            renderer.Draw(vao, ib, shader);*/
//
//            /* Swap front and back buffers */
//            glfwSwapBuffers(window);
//
//            /* Poll for and process events */
//            glfwPollEvents();
//        }
//    }
//
//    glfwTerminate();
//    return 0;
//}
//
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//    windowWidth = width;
//    windowHeight = height;
//}
//
//void mouse_movement_callback(GLFWwindow* window, double mouseXPos, double mouseYPos)
//{
//    float xPos = static_cast<float>(mouseXPos);
//    float yPos = static_cast<float>(mouseYPos);
//
//    if (firstMouse)
//    {
//        mouseLastX = xPos;
//        mouseLastY = yPos;
//        firstMouse = false;
//    }
//
//    float xoffset = xPos - mouseLastX;
//    float yoffset = mouseLastY - yPos; // reversed since y-coordinates go from bottom to top
//    mouseLastX = xPos;
//    mouseLastY = yPos;
//
//    float sensitivity = 0.1f; // change this value to your liking
//    xoffset *= sensitivity;
//    yoffset *= sensitivity;
//
//    yaw += xoffset;
//    pitch += yoffset;
//
//    // make sure that when pitch is out of bounds, screen doesn't get flipped
//    if (pitch > 89.0f)
//        pitch = 89.0f;
//    if (pitch < -89.0f)
//        pitch = -89.0f;
//
//    glm::vec3 front;
//    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
//    front.y = sin(glm::radians(pitch));
//    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
//    cameraFront = glm::normalize(front);
//}
//
//void processInput(GLFWwindow* window)
//{
//    const float cameraSpeed = 2.5f * deltaTime;
//    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//        cameraPos += cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//        cameraPos -= cameraSpeed * cameraFront;
//    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
//}
