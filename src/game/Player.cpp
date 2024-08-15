#include<iostream>

#include "Player.h"
#include"Transform.h"
#include "Game.h"

Player::Player(Camera* camera) 
    : Entity(), camera(camera) 
{ 
    transform.scale = glm::vec3(1.0f);

    collision_channel = Collision_Channel::Player;
}

void Player::update(GLFWwindow* window, float deltaTime) 
{
    processKeyboard(window, deltaTime);
    updateCameraPosition();

    Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
    if (hit_actor != nullptr) 
    {
        transform.position = glm::vec3(10);
    }
}

void Player::processKeyboard(GLFWwindow* window, float deltaTime)
{
    //std::cout << transform.position.x << transform.position.y << transform.position.z << std::endl;

    const float playerSpeed = 20 * deltaTime;
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