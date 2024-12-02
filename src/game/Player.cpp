#include<iostream>

#include <MeshManager.h>
#include <ShaderManager.h>

#include "Player.h"
#include"Transform.h"
#include "Game.h"
#include "imgui/imgui.h"
#include "AudioManager.h"

#include "enemies/boss/BossCage.h"

Player::Player(Camera* camera, GLFWwindow* window)
    : Entity(), camera(camera), physicsbody(), window(window)
{ 
    transform.scale = glm::vec3(0.4f, 0.7f, 0.4f);
    transform.position = transform.getUp() * (transform.scale.y / 2.f);
    physicsbody.bGravity = true;
    collision_channel = Collision_Channel::Player;

    this->meshRenderer = new MeshRenderer(meshManager->getMesh("cube"), shaderManager->getShader("mesh"), Camera::mainCamera);
    meshRenderer->setColor(glm::vec3(1.f));
}

void Player::update(float deltaTime) 
{
    physicsbody.update();

    if (!canInput) return;
    processKeyboard(window, deltaTime);
    processAudioInput(window);

    tiltCamera(window, deltaTime);

    updateCameraPosition();
    checkCollision();

    update_shield(deltaTime);
    update_dash(deltaTime);
    update_iframe(deltaTime);

    camera->updateShake(deltaTime);
}

void Player::checkCollision() 
{
    //return;

    Entity* hit_actor = game->get_coliding_entity(this, Collision_Channel::Enemy);
    if (hit_actor != nullptr && !bIsInvincible)
    {
        auto gameOverSoundEffect = audioManager->getAudioClip("GameOver.mp3");
        audioManager->playSound(gameOverSoundEffect, transform.position, 0.4f);

        if (bIsShieldAlive) {
            bIsShieldAlive = false;
            bIsInvincible = true;
            invincibilityCooldownTimer = invincibilityCooldown;
            camera->invokeScreenShake(.07f, 0.7f);
        }
        else {
            die();
        }
    }
}

void Player::die() {
    game->gameOver();
}

void Player::reset()
{
    shooter.emptyAllQueues();

    transform.position = glm::vec3(0.0f);

    bIsShieldAlive = true;
    bIsInvincible = false;

    transform.position = transform.getUp() * (transform.scale.y / 2.f);
    transform.rotation = glm::quat(1, 0, 0, 0);

    lastX = camera->getScreenWidth() / 2.0;
    lastY = camera->getScreenHeight() / 2.0;
    reset_pitch();
    physicsbody.velocity = glm::vec3(0);
    physicsbody.acceleration = glm::vec3(0);
    firstMouse = true;
}

void Player::update_shield(float dt)
{
    if (bIsShieldAlive) return;
        shieldCooldownTimer -= dt;
        if (shieldCooldownTimer <= 0.0f) {
            shieldCooldownTimer = shieldCooldown;
            bIsShieldAlive = true;
        }
}

void Player::update_dash(float dt)
{
    if (bCanDash) return;
        dashCooldownTimer -= dt;
        if (dashCooldownTimer <= 0.0f) {
            bCanDash = true;
            dashCooldownTimer = 0.0f;
        }
}

void Player::update_iframe(float dt)
{
    if (!bIsInvincible) return;
        invincibilityCooldownTimer -= dt;
        if (invincibilityCooldownTimer <= 0.0f) {
            bIsInvincible = false;
            invincibilityCooldownTimer = invincibilityCooldown;
        }
}

void Player::processKeyboard(GLFWwindow* window, float deltaTime)
{   
    // movement
    const float playerSpeed = speed * game->playerSpeedMultiplier * deltaTime;
    physicsbody.acceleration = glm::vec3(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        physicsbody.acceleration += (playerSpeed * transform.getForward()) * glm::vec3(1, 0, 1);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        physicsbody.acceleration -= (playerSpeed * transform.getForward()) * glm::vec3(1, 0, 1);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        physicsbody.acceleration -= glm::normalize(glm::cross(transform.getForward(), getWorldUp())) * playerSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        physicsbody.acceleration += glm::normalize(glm::cross(transform.getForward(), getWorldUp())) * playerSpeed;

    // debug
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        game->addPlayerXP(10);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        game->reset();
        this->reset();
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        game->startCredits();
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        game->changeState(GameStateManager::BossFight);
    }

    // jumping and dampening
    bool bIsGrounded = transform.position.y < 0 + playerHeight + 0.1f;
    physicsbody.dampening = bIsGrounded ? 5.f : 2.f;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && bIsGrounded)
    {
        physicsbody.add_force(glm::vec3(0, 10000 * deltaTime, 0));
    }

    // dashing
    float vMagnitude = glm::length(physicsbody.velocity);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && bCanDash && vMagnitude > 4)
    {
        glm::vec3 dashDirection = glm::normalize(physicsbody.velocity);
        if (glm::length(dashDirection) > 0.0f)
        {
            const float dashSpeed = 40000.0f * game->playerDashMultiplier;
            physicsbody.add_force(dashDirection * dashSpeed * deltaTime);

            dashCooldownTimer = dashCooldown;
            bCanDash = false;
        }
    }

    // apply velocity

    if (game->bossFightController->getCage() != nullptr) {
        BossCage* cage = game->bossFightController->getCage();
        glm::vec2 minBounds = cage->minBounds;
        glm::vec2 maxBounds = cage->maxBounds;

        transform.position.x = glm::clamp(transform.position.x, minBounds.x + 0.5f, maxBounds.x - 0.5f);
        transform.position.z = glm::clamp(transform.position.z, minBounds.y + 0.5f, maxBounds.y - 0.5f);
    }

    transform.position += physicsbody.velocity * deltaTime;
    transform.position.y = glm::clamp(transform.position.y, 0.0f + playerHeight, 100.0f);
}

void Player::tiltCamera(GLFWwindow* window, float deltaTime)
{
    static float currentTilt = 0.0f;
    static float lastTilt = 0.0f;

    const float rotationSpeed = 10.0f;
    const float maxTilt = 5.0f;
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
    if (!canInput) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        shooter.shootDefault(camera->transform.position + camera->transform.getForward() * glm::vec3(0.15), camera->transform.getForward(), camera->transform.getUp());
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        shooter.shootCardFromQueue(camera->transform.position, camera->transform.getForward(), camera->transform.getUp());
    }
}

void Player::mouse_movement_callback(float xPos, float yPos)
{
    //if (game->bGameOver) return;

    if (!canInput) return;

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
    camera->transform.rotation = yRotate * camera->transform.rotation;
    transform.rotation = yRotate * camera->transform.rotation;

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
    glm::quat xRotate = glm::angleAxis(glm::radians(yOffset), camera->transform.getRight());
    camera->transform.rotation = xRotate * camera->transform.rotation;
}

void Player::updateCameraPosition() 
{
    camera->transform.position = transform.position;
}

void Player::processAudioInput(GLFWwindow* window)
{
    const float volume = 0.05f;

    bool isWPressed = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);
    if (isWPressed && !prevWState)
    {
        auto clip = audioManager->getAudioClip("MoveUp.mp3");
        if (clip)
            audioManager->playSound(clip, transform.position, volume);
    }

    bool isSPressed = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS);
    if (isSPressed && !prevSState)
    {
        auto clip = audioManager->getAudioClip("MoveBack.mp3");
        if (clip)
            audioManager->playSound(clip, transform.position, volume);
    }

    bool isAPressed = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
    if (isAPressed && !prevAState)
    {
        auto clip = audioManager->getAudioClip("MoveLeft.mp3");
        if (clip)
            audioManager->playSound(clip, transform.position, volume);
    }

    bool isDPressed = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS);
    if (isDPressed && !prevDState)
    {
        auto clip = audioManager->getAudioClip("MoveRight.mp3");
        if (clip)
            audioManager->playSound(clip, transform.position, volume);
    }

    prevWState = isWPressed;
    prevSState = isSPressed;
    prevAState = isAPressed;
    prevDState = isDPressed;
}