#pragma once

#include<glew/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Transform.h"
#include "Camera.h"
#include "Entity.h"

class Camera;

class Player : public Entity
{
public:
	Player(Camera* camera);

	void update(GLFWwindow* window, float deltaTime);

	void processKeyboard(GLFWwindow* window, float deltaTime);
	void updateCameraPosition();

	Transform transform;

	float playerSpeed = 2.0f;

private:
	Camera* camera;
};