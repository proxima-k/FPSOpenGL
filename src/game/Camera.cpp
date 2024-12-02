#include "Camera.h"
#include "Game.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <iostream>

Camera* Camera::mainCamera = nullptr;

Camera::Camera(glm::vec3 worldUp, unsigned int screenWidth, unsigned int screenHeight)
    : worldUp(worldUp), screenWidth(screenWidth), screenHeight(screenHeight) {}

void Camera::invokeScreenShake(float intensity, float duration) {
    shakeIntensity = intensity;
    shakeDuration = duration;
    shakeTimeLeft = duration;
    bScreenIsShaking = true;
}

void Camera::updateShake(float deltaTime) {
    if (bScreenIsShaking) {
        shakeTimeLeft -= deltaTime;

        if (shakeTimeLeft <= 0.0f) {
            bScreenIsShaking = false;
        }
        else {
            float shakeAmount = shakeIntensity * (shakeTimeLeft / shakeDuration);
            float offsetX = shakeAmount * std::sin(shakeTimeLeft * shakeFrequency * 3.0f);
            float offsetY = shakeAmount * std::cos(shakeTimeLeft * shakeFrequency * 3.0f);

            transform.position.x += offsetX;
            transform.position.y += offsetY;

            shakeFrequency = std::max(shakeFrequency, 20.0f);
        }
    }
}


//void Camera::lookAt(glm::vec3 targetPosition)
//{
//    glm::vec3 forward = glm::normalize(targetPosition - transform.position);
//    glm::vec3 right = glm::normalize(glm::cross(worldUp, forward)); 
//    glm::vec3 up = glm::cross(forward, right);
//
//    // Create a rotation matrix from the basis vectors
//    glm::mat3 rotationMatrix = glm::mat3(right, up, forward);
//
//    // Convert rotation matrix to quaternion
//    transform.rotation = glm::quat_cast(rotationMatrix);
//}


/*
void Camera::processMouseMovement(float xPos, float yPos)
{
    std::cout << xPos << " " << yPos << std::endl;
    if (game->bGameOver) return;

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
*/
