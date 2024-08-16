#include "Physicsbody.h"
#include "glew/glew.h"
#include "GLFW/glfw3.h"

void Physicsbody::update() 
{
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrameTime;
    lastFrameTime = currentFrame;
    
    float gravity = -9.81f;
    float gravityAmplitude = 120.f;
    if (bGravity) 
    {
        acceleration.y += gravity * gravityAmplitude * deltaTime;
    }

    velocity += acceleration * deltaTime;

    double dampingFactor = 1.f - (dampening * deltaTime);

    if (dampingFactor < 0) 
        dampingFactor = 0;

    velocity *= dampingFactor;
}