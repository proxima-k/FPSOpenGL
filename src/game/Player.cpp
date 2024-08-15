#include<iostream>

#include "Player.h"
#include"Transform.h"
#include "Game.h"

Player::Player(Camera* camera)
    : Entity(), camera(camera), physicsbody()
{ 
    transform.scale = glm::vec3(1.0f);
    physicsbody.bGravity = true;
    collision_channel = Collision_Channel::Player;
}

void Player::update(GLFWwindow* window, float deltaTime) 
{
    physicsbody.update();
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
    const float playerSpeed = 800 * deltaTime;
    physicsbody.acceleration = glm::vec3(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        physicsbody.acceleration += playerSpeed * camera->getCameraForward();
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        physicsbody.acceleration -= playerSpeed * camera->getCameraForward();
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        physicsbody.acceleration -= glm::normalize(glm::cross(camera->getCameraForward(), camera->getCameraUp())) * playerSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        physicsbody.acceleration += glm::normalize(glm::cross(camera->getCameraForward(), camera->getCameraUp())) * playerSpeed;
    
    bool bIsGrounded = transform.position.y < 0 + playerHeight + 0.1f;
    if (bIsGrounded)
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            physicsbody.add_force(glm::vec3(0, 10000 * deltaTime, 0));
    }

    transform.position += physicsbody.velocity * deltaTime;
    transform.position.y = glm::clamp(transform.position.y, 0.0f + playerHeight, 100.0f);
}

void Player::updateCameraPosition() 
{
    camera->transform.position = transform.position;
}