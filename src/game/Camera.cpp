#include "Camera.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/glm.hpp>
#include<iostream>
#include<iomanip>

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

    glm::quat yRotate = glm::angleAxis(glm::radians(-xOffset), glm::vec3(0, 1, 0));
    transform.rotation = yRotate * transform.rotation;

    std::cout << currentPitch << std::endl;

    float newPitch = currentPitch + yOffset;

    if (newPitch < -89.f) {
        yOffset = -89.f - currentPitch;
        currentPitch = -89.f;
    }
    else if (newPitch > 89.f) {
        yOffset = 89.f - currentPitch;
        currentPitch = 89.f;
    }
    else {
        currentPitch += yOffset;
    }

    glm::quat xRotate = glm::angleAxis(glm::radians(yOffset), transform.getRight());
    transform.rotation = xRotate * transform.rotation;


    std::cout << std::setprecision(2) << std::fixed;
    
    /*std::cout << "Forward:" << std::endl;
    std::cout << "X: " << transform.getForward().x << " Y: " << transform.getForward().y << " Z: " << transform.getForward().z << std::endl;
    std::cout << "Right:" << std::endl;
    std::cout << "X: " << transform.getRight().x << " Y: " << transform.getRight().y << " Z: " << transform.getRight().z << std::endl;
    std::cout << "Up:" << std::endl;
    std::cout << "X: " << transform.getUp().x << " Y: " << transform.getUp().y << " Z: " << transform.getUp().z << std::endl;
    std::cout << std::endl;*/

    /*
    transform.rotation.y += xoffset; // Yaw
    transform.rotation.x += yoffset; // Pitch

    // Constrain pitch to avoid gimbal lock
    if (transform.rotation.x > 89.0f)
        transform.rotation.x = 89.0f;
    if (transform.rotation.x < -89.0f)
        transform.rotation.x = -89.0f;
    */

    //glm::vec3 eulerAngles = glm::eulerAngles(transform.rotation);
    //std::cout << "X: " << eulerAngles.x << " Y: " << eulerAngles.y << " Z: " << eulerAngles.z << std::endl;
    //std::cout << "X: " << transform.rotation.x << " Y: " << transform.rotation.y << " Z: " << transform.rotation.z << " W: " << transform.rotation.w << std::endl;

    cameraForward = transform.getForward();
}
