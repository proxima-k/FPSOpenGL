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

    audioManager.init();
}

void Player::update(GLFWwindow* window, float deltaTime) 
{
    physicsbody.update();

    processKeyboard(window, deltaTime);
    processAudioInput(window);

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

    /*Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
    if (hit_actor != nullptr)
    {
        die();
    }*/

    Entity* hit_actor2 = game->get_coliding_entity(this, Collision_Channel::XP);
    if (hit_actor2 != nullptr)
    {
        hit_actor2->destroy();
    }
}

void Player::die()

{
    game->GameOver();
}

void Player::processKeyboard(GLFWwindow* window, float deltaTime)
{
    // movement
    const float playerSpeed = speed * game->playerSpeedMultiplier * deltaTime;
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
            const float dashSpeed = 40000.0f * game->playerDashMultiplier;
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
        targetTilt = 0.0f;
    }

    currentTilt = glm::mix(currentTilt, targetTilt, deltaTime * rotationSpeed);
    currentTilt = glm::clamp(currentTilt, -maxTilt, maxTilt);

    float tiltDifference = currentTilt - lastTilt;

    glm::quat tiltQuat = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(tiltDifference)));

    camera->transform.rotation = glm::normalize(glm::quat(camera->transform.rotation) * tiltQuat);

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
        shooter.shootCardFromQueue(camera->transform.position, camera->transform.getForward(), camera->transform.getUp());
    }
}

void Player::updateCameraPosition() 
{
    camera->transform.position = transform.position;
}

void Player::processAudioInput(GLFWwindow* window)
{
    bool isWPressed = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    if (isWPressed && !prevWState)
    {
        auto clip = audioManager.getAudioClip("MoveUp.mp3");
        if (clip)
            audioManager.playSound(clip, transform.position, 1);
    }

    bool isSPressed = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    if (isSPressed && !prevSState)
    {
        auto clip = audioManager.getAudioClip("MoveBack.mp3");
        if (clip)
            audioManager.playSound(clip, transform.position, 1);
    }

    bool isAPressed = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    if (isAPressed && !prevAState)
    {
        auto clip = audioManager.getAudioClip("MoveLeft.mp3");
        if (clip)
            audioManager.playSound(clip, transform.position, 1);
    }

    bool isDPressed = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
    if (isDPressed && !prevDState)
    {
        auto clip = audioManager.getAudioClip("MoveRight.mp3");
        if (clip)
            audioManager.playSound(clip, transform.position, 1);
    }

    prevWState = isWPressed;
    prevSState = isSPressed;
    prevAState = isAPressed;
    prevDState = isDPressed;
}