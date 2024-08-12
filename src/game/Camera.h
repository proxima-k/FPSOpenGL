#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glew/glew.h>
#include <GLFW/glfw3.h>

#include "Transform.h"

class Camera {
public:
    Camera(Transform transform, glm::vec3 up);

    void update(GLFWwindow* window, float deltaTime);
    void processMouseMovement(float xPos, float yPos);

    Transform transform;
private:
    glm::vec3 cameraForward;
    glm::vec3 cameraUp;

    float lastX;
    float lastY;
    bool firstMouse;

    void processKeyboard(GLFWwindow* window, float deltaTime);
};

#endif
