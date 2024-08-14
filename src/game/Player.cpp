#include<iostream>


#include "Player.h"
#include"Transform.h"

Player::Player(Camera* camera) 
    : camera(camera) { }

void Player::update(GLFWwindow* window, float deltaTime) 
{
    processKeyboard(window, deltaTime);
}

void Player::processKeyboard(GLFWwindow* window, float deltaTime)
{
    const float playerSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        transform.position += playerSpeed * camera->getCameraForward();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        transform.position -= playerSpeed * camera->getCameraForward();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        transform.position -= glm::normalize(glm::cross(camera->getCameraForward(), camera->getCameraUp())) * playerSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        transform.position += glm::normalize(glm::cross(camera->getCameraForward(), camera->getCameraUp())) * playerSpeed;
}

void Player::updateCameraPosition() 
{
    camera->transform.position = transform.position;
}