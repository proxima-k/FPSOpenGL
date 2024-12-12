#pragma once

#include<glew/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Camera.h"
#include "Entity.h"
#include "Physicsbody.h"
#include "shooting/Shooter.h"

class Camera;

class Player : public Entity
{
public:
	Player(Camera* camera, GLFWwindow* window);

	void update(float deltaTime);
	void processKeyboard(GLFWwindow* window, float deltaTime);
	void processAudioInput(GLFWwindow* window);
	void tiltCamera(GLFWwindow* window, float deltaTime);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void mouse_movement_callback(float xPos, float yPos);
	void updateCameraPosition();
	void checkCollision();
	void damage();
	void die();
	void reset();

	void update_shield(float dt);
	void update_dash(float dt);
	void update_iframe(float dt);

	void reset_pitch() { 
		currentTilt = 0.0f;
		lastTilt = 0.0f;
		currentPitch = 0.0f;
	}
	glm::vec3 getWorldUp() const { return glm::vec3(0, 1, 0); }

	Camera* getCamera() const { return camera; }

	Shooter shooter;

	bool prevWState;
	bool prevSState;
	bool prevAState;
	bool prevDState;

	float playerHeight = transform.scale.y / 2;
	float speed = 1600;
	Physicsbody physicsbody;

	bool canInput = true;
	bool canDrawBody = false;

	bool bIsShieldAlive;

	float shieldCooldown = 5.0f;
	float shieldCooldownTimer = 0.0f;

	bool firstMouse = true;

private:
	Camera* camera = nullptr;
	GLFWwindow* window = nullptr;

	glm::vec3 lastVelocity = glm::vec3(0.0f);

	float dashCooldown = 2.0f;
	float dashCooldownTimer = 0.0f;

	float invincibilityCooldown = 0.5f;
	float invincibilityCooldownTimer = 0.0f;

	bool bCanDash = true;
	bool bUsedDash = false;
	bool bIsInvincible;
	// camera settings
	float lastX;
	float lastY;
	float currentPitch = 0;

	float currentTilt = 0.0f;
	float lastTilt = 0.0f;

	bool canDash = true;

	const float DASH_FOV_TIME = 0.8f;
	float dashFOVTimer = 0.f;
};