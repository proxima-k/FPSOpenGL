#include<iostream>

#include "Player.h"
#include"Transform.h"
#include "Game.h"
#include "imgui/imgui.h"

Player::Player(Camera* camera)
    : Entity(), camera(camera), physicsbody()
{ 
    transform.scale = glm::vec3(0.5f);
    physicsbody.bGravity = true;
    collision_channel = Collision_Channel::Player;
}

void Player::update(GLFWwindow* window, float deltaTime) 
{
    physicsbody.update();
    shooter.update(deltaTime);

    processKeyboard(window, deltaTime);
    tiltCamera(window, deltaTime);
    updateCameraPosition();
    checkCollision();

    if (!canDash)
    {
        dashCooldownTimer -= deltaTime;
        if (dashCooldownTimer <= 0.0f)
        {
            canDash = true;
            dashCooldownTimer = 0.0f;
        }
    }
}

void Player::checkCollision() 
{
    Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
    if (hit_actor != nullptr)
    {
        transform.position = glm::vec3(10);
    }

    Entity* hit_actor2 = game->get_coliding_entity(this, Collision_Channel::XP);
    if (hit_actor2 != nullptr)
    {
        hit_actor2->destroy();
    }
}

void Player::processKeyboard(GLFWwindow* window, float deltaTime)
{
    // movement
    const float playerSpeed = speed * deltaTime;
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

void Player::tiltCamera(GLFWwindow* window, float deltaTime)
{
    static float currentTilt = 0.0f;
    static float lastTilt = 0.0f;

    float rotationSpeed = 10.0f;
    float maxTilt = 5.0f;
    float targetTilt = 0.0f;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        targetTilt = -maxTilt;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        targetTilt = maxTilt;
    }
    else
    {
        targetTilt = 0.0f;  // no input, return to 0 tilt
    }

    // lerp current tilt towards the target tilt
    currentTilt = glm::mix(currentTilt, targetTilt, deltaTime * rotationSpeed);

    // clamp the tilt to the maximum allowed tilt range
    currentTilt = glm::clamp(currentTilt, -maxTilt, maxTilt);

    // calculate the difference in tilt from the last frame
    float tiltDifference = currentTilt - lastTilt;

    // apply the tilt relative to the current camera rotation
    glm::quat tiltQuat = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(tiltDifference)));

    // combine the tilt rotation with the existing camera rotation correctly
    camera->transform.rotation = glm::normalize(glm::quat(camera->transform.rotation) * tiltQuat);

    // update last tilt value for the next frame
    lastTilt = currentTilt;

}

void Player::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        shooter.shootDefault(camera->transform.position, camera->transform.getForward(), camera->transform.getUp());
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        // shooter should have a current card
        // so everytime the it shoots, it will do a card.launch() with given values 
        // like starting position, target aiming location
        shooter.shootRandom(camera->transform.position, camera->transform.getForward(), camera->transform.getUp());
    }
}

void Player::updateCameraPosition() 
{
    camera->transform.position = transform.position;
}