#include<iostream>

#include "Player.h"
#include"Transform.h"
#include "Game.h"
#include "imgui/imgui.h"

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

    shooter.update(deltaTime);
    Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
    if (hit_actor != nullptr) 
    {
        transform.position = glm::vec3(10); 
    }

    if (!canDash)
    {
        dashCooldownTimer -= deltaTime;
        if (dashCooldownTimer <= 0.0f)
        {
            canDash = true;
            dashCooldownTimer = 0.0f; // Reset the timer
        }
    }
}

void Player::processKeyboard(GLFWwindow* window, float deltaTime)
{
    // movement
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

    // jumping and dampening
    bool bIsGrounded = transform.position.y < 0 + playerHeight + 0.1f;
    physicsbody.dampening = bIsGrounded ? 5.f : 2.f;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && bIsGrounded)
    {
        physicsbody.add_force(glm::vec3(0, 10000 * deltaTime, 0));
    }

    // dashing
    float vMagnitude = glm::length(physicsbody.velocity);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && canDash && vMagnitude > 4)
    {
        glm::vec3 dashDirection = glm::normalize(physicsbody.velocity);
        if (glm::length(dashDirection) > 0.0f)
        {
            const float dashSpeed = 40000.0f;
            physicsbody.add_force(dashDirection * dashSpeed * deltaTime);

            dashCooldownTimer = dashCooldown;
            canDash = false;
        }
    }

    // apply velocity
    transform.position += physicsbody.velocity * deltaTime;
    transform.position.y = glm::clamp(transform.position.y, 0.0f + playerHeight, 100.0f);
}

void Player::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        // shooter should have a current card
        // so everytime the it shoots, it will do a card.launch() with given values 
        // like starting position, target aiming location
        shooter.shoot(camera->transform.position, camera->transform.getForward(), camera->transform.getUp());
    }
}

void Player::updateCameraPosition() 
{
    camera->transform.position = transform.position;
}