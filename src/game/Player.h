#pragma once

#include<glew/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Camera.h"
#include "Entity.h"
#include "Physicsbody.h"
#include "shooting/Shooter.h"
#include "AudioManager.h"

class Camera;

class Player : public Entity
{
public:
	Player(Camera* camera);

	void update(GLFWwindow* window, float deltaTime);
	void processKeyboard(GLFWwindow* window, float deltaTime);
	void processAudioInput(GLFWwindow* window);
	void tiltCamera(GLFWwindow* window, float deltaTime);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void updateCameraPosition();
	void checkCollision();
	void die();

	Camera* getCamera() const { return camera; }

	Shooter shooter;

	bool prevWState;
	bool prevSState;
	bool prevAState;
	bool prevDState;

	float playerHeight = transform.scale.y / 2;
	float speed = 1600;

private:
	Physicsbody physicsbody;
	Camera* camera;

	glm::vec3 lastVelocity = glm::vec3(0.0f);

	float dashCooldown = 2.0f;
	float dashCooldownTimer = 0.0f;

	bool canDash = true;
};