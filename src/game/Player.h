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
	void die();
	void reset();

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

private:
	Camera* camera = nullptr;
	GLFWwindow* window = nullptr;

	glm::vec3 lastVelocity = glm::vec3(0.0f);

	float dashCooldown = 2.0f;
	float dashCooldownTimer = 0.0f;

	// camera settings
	float lastX;
	float lastY;
	bool firstMouse = true;
	float currentPitch = 0;

	bool canDash = true;
};