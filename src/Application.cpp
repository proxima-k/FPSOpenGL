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

#include<openAL/al.h>
#include<openAL/alc.h>
#include<openAL/dr_mp3.h>

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

#include "engine/Debug.h"
#include "engine/Logger.h"

#include "game/Grid.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_movement_callback(GLFWwindow* window, double xPos, double yPos);

// window settings
unsigned int windowWidth = 1920;
unsigned int windowHeight = 1080;

// mouse input handling
float mouseLastX = windowWidth / 2.f;
float mouseLastY = windowHeight / 2.f;

// time between loops
bool firstMouse = true;
float deltaTime = 0.f;
float lastFrameTime = 0.f;

Camera camera(glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), windowWidth, windowHeight);
Player player(&camera);
Game* game;

#define OpenAL_ErrorCheck(message)\
{\
    ALenum error = alGetError();\
    if (error != AL_NO_ERROR)\
	{\
		std::cout << "OpenAL Error: " << error << std::endl;\
		std::cout << message << std::endl;\
	}\
}

#define alec(FUNCTION_CALL)\
FUNCTION_CALL;\
OpenAL_ErrorCheck(#FUNCTION_CALL)

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
        Grid floorGrid(1, 1, 16);
        Shader gridShader("res/shaders/grid.shader");
        floorGrid.setShader(&gridShader);
        floorGrid.setCamera(&camera);
        floorGrid.setPlayer(&player);

        // Entity (mesh path, shader, camera)
        std::vector<float> vertices = Mesh::getMeshVerticesFromObjFile("res/models/cube.obj");
        Mesh teapotMesh(vertices);


        Shader meshShader("res/shaders/Basic.shader");
        meshShader.Bind();

        Entity* teapotEntity = game->spawn_entity<CubeEnemy>(glm::vec3(1), &teapotMesh, &meshShader, &camera);

        // get default audio device
        const ALchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
        ALCdevice* device = alcOpenDevice(defaultDeviceString);
        if (!device) 
        {
            std::cout << "Failed to open device" << std::endl;
        }
        else 
        {
        std::cout << "Opened device: " << alcGetString(device, ALC_DEVICE_SPECIFIER) << std::endl;
        }

        // create context from device
        ALCcontext* context = alcCreateContext(device, nullptr);

        // activate context so that openAL state modifications affect this context
        if(!alcMakeContextCurrent(context))
		{
			std::cout << "Failed to make context current" << std::endl;
		}

        // create a listener in 3d space
        alec(alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f));
        alec(alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f));

        // player camera orientation
        ALfloat forwardAndUpVectors[] = {
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f
        };
        alec(alListenerfv(AL_ORIENTATION, forwardAndUpVectors));

        //create buffers that hold audio data

        drmp3 mp3;
        if(!drmp3_init_file(&mp3, "C:/Projects/FPSOpenGL/res/audio/test.mp3", nullptr))
		{
			std::cout << "Failed to load mp3 file" << std::endl;
		}
        
        // Retrieve audio data properties
        drmp3_uint32 channels = mp3.channels;
        drmp3_uint32 sampleRate = mp3.sampleRate;

        // Calculate the size of the buffer to hold the audio data
        drmp3_uint64 totalFrames = drmp3_get_pcm_frame_count(&mp3);
        std::vector<float> pcmData(totalFrames * channels);

        // Read the PCM frames
        drmp3_uint64 framesRead = drmp3_read_pcm_frames_f32(&mp3, totalFrames, pcmData.data());

        // Generate and fill OpenAL buffer
        ALuint buffer;
        alec(alGenBuffers(1, &buffer));

        ALenum format;
        if (channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else if (channels == 2)
        {
            format = AL_FORMAT_STEREO16;
        }
        else
        {
            std::cerr << "Unsupported channel count: " << channels << std::endl;
            return -1;
        }

        alec(alBufferData(buffer, format, pcmData.data(), static_cast<ALsizei>(framesRead * channels * sizeof(float)), sampleRate));

        // Clean up
        drmp3_uninit(&mp3);
        alcDestroyContext(context);
        alcCloseDevice(device);


        glEnable(GL_DEPTH_TEST);

        /* Loop until the user closes the window */
        GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwTerminate();

            double currentFrame = glfwGetTime();
            deltaTime = currentFrame - lastFrameTime;
            lastFrameTime = currentFrame;

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //ImGui::ShowDemoWindow(&show_demo_window);
            //logger.draw("Logger", &show_log_window);

            player.update(window, deltaTime);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            game->update();
            game->render();

            floorGrid.update();
            floorGrid.draw();

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