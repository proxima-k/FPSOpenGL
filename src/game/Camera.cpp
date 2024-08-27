#include "Camera.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/glm.hpp>
#include<iostream>

Camera* Camera::mainCamera = nullptr;

Camera::Camera(Transform transform, glm::vec3 up, unsigned int screenWidth, unsigned int screenHeight)
    : transform(transform), cameraUp(up), firstMouse(true), screenWidth(screenWidth), screenHeight(screenHeight)
{
    cameraForward = glm::vec3(0.0f, 0.0f, 1.0f);
    transform.rotation = glm::vec3(0, 0, 0);
    lastX = 640.0f / 2.0;  // assuming initial window width of 640
    lastY = 640.0f / 2.0;  // assuming initial window height of 640
}

void Camera::processMouseMovement(float xPos, float yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = yPos - lastY;
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f; // Adjust sensitivity as needed
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // Rotate the Yaw of the camera (looking left and right)
    glm::quat yRotate = glm::angleAxis(glm::radians(-xOffset), glm::vec3(0, 1, 0));
    transform.rotation = yRotate * transform.rotation;

    // constrain pitch to avoid overturn
    float newPitch = currentPitch + yOffset;
    if (newPitch < -89.f) {
        yOffset = -89.f - currentPitch;
    }
    else if (newPitch > 89.f) {
        yOffset = 89.f - currentPitch;
    }
    currentPitch += yOffset;

    // Rotate the Pitch of the camera (looking up and down)
    glm::quat xRotate = glm::angleAxis(glm::radians(yOffset), transform.getRight());
    transform.rotation = xRotate * transform.rotation;

    cameraForward = transform.getForward();
}
