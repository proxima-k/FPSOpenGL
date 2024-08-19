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

    ImGui::Begin("Physics Body Controls");

    ImGui::Text("Physics Body Values");
    ImGui::Separator();

    // Display and modify gravity
    ImGui::Checkbox("Gravity Enabled", &physicsbody.bGravity);

    // Display velocity and acceleration
    ImGui::Text("Velocity: (%.3f, %.3f, %.3f)", physicsbody.velocity.x, physicsbody.velocity.y, physicsbody.velocity.z);
    ImGui::Text("Acceleration: (%.3f, %.3f, %.3f)", physicsbody.acceleration.x, physicsbody.acceleration.y, physicsbody.acceleration.z);

    // Modify damping and friction
    ImGui::SliderFloat("Damping", &physicsbody.dampening, 0.0f, 10.0f);

    ImGui::End();

    // Rendering
    ImGui::Render();

    Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
    if (hit_actor != nullptr) 
    {
        //transform.position = glm::vec3(10); 
    }
}

void Player::processKeyboard(GLFWwindow* window, float deltaTime)
{
    // shooting system
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT)) {
        // shooter.Shoot()
        // shooter should have a current card
        // so everytime the it shoots, it will do a card.launch() with given values 
        // like starting position, target aiming location

        shooter.shoot(camera->transform.position, camera->transform.getForward());
    }

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
    
    // jumping and friction
    bool bIsGrounded = transform.position.y < 0 + playerHeight + 0.1f;
    if (bIsGrounded)
    {
        physicsbody.dampening = 5.f;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            physicsbody.add_force(glm::vec3(0, 10000 * deltaTime, 0));
    }
    else
        physicsbody.dampening = 2.f;

    transform.position += physicsbody.velocity * deltaTime;
    transform.position.y = glm::clamp(transform.position.y, 0.0f + playerHeight, 100.0f);
}

void Player::updateCameraPosition() 
{
    camera->transform.position = transform.position;
}