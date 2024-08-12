#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glew/glew.h>
#include <GLFW/glfw3.h>

class Camera {
public:
    Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);

    void update(GLFWwindow* window, float deltaTime);
    void processMouseMovement(float xPos, float yPos);

    glm::vec3 getPosition() const { return cameraPos; }
    glm::vec3 getFront() const { return cameraFront; }

private:
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;

    float yaw;
    float pitch;

    float lastX;
    float lastY;
    bool firstMouse;

    void processKeyboard(GLFWwindow* window, float deltaTime);
};

#endif
