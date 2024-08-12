#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<glm/glm.hpp>
#include<iostream>

Camera::Camera(Transform transform, glm::vec3 up)
    : transform(transform), cameraUp(up), firstMouse(true)
{
    cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);
    transform.rotation = glm::vec3(0, 0, 0);
    lastX = 640.0f / 2.0;  // assuming initial window width of 640
    lastY = 640.0f / 2.0;  // assuming initial window height of 640
}

void Camera::update(GLFWwindow* window, float deltaTime)
{
    processKeyboard(window, deltaTime);
}

void Camera::processKeyboard(GLFWwindow* window, float deltaTime)
{
    const float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += cameraSpeed * cameraForward;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position -= cameraSpeed * cameraForward;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform.position -= glm::normalize(glm::cross(cameraForward, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += glm::normalize(glm::cross(cameraForward, cameraUp)) * cameraSpeed;


}

void Camera::processMouseMovement(float xPos, float yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xoffset = xPos - lastX;
    float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
    lastX = xPos;
    lastY = yPos;

    float sensitivity = 0.1f; // Adjust sensitivity as needed
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    transform.rotation.x += xoffset; // Yaw
    transform.rotation.y += yoffset; // Pitch

    // Constrain pitch to avoid gimbal lock
    if (transform.rotation.y > 89.0f)
        transform.rotation.y = 89.0f;
    if (transform.rotation.y < -89.0f)
        transform.rotation.y = -89.0f;

    cameraForward = transform.getForward();
}
