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
	Player(Camera* camera);

	void update(GLFWwindow* window, float deltaTime);

	void processKeyboard(GLFWwindow* window, float deltaTime);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void updateCameraPosition();

	float playerHeight = transform.scale.y / 2;

	Shooter shooter;

private:
	Camera* camera;
	Physicsbody physicsbody;

	glm::vec3 lastVelocity = glm::vec3(0.0f);
	bool canDash = true;
	float dashCooldown = 2.0f;
	float dashCooldownTimer = 0.0f;
};