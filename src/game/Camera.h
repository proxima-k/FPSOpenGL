#ifndef CAMERA_H
#define CAMERA_H

#include <glew/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Transform.h"

class Camera {
public:
    static Camera* mainCamera;
    
    Camera(Transform transform, glm::vec3 up, unsigned int screenWidth, unsigned int screenHeight);

    void processMouseMovement(float xPos, float yPos);

    glm::vec3 getCameraForward() const { return cameraForward; }
    glm::vec3 getCameraUp() const { return cameraUp; }

    float getResolutionRatio() { return (float)screenWidth / screenHeight; }

    Transform transform;

private:
    glm::vec3 cameraForward;
    glm::vec3 cameraUp;

    float lastX;
    float lastY;
    bool firstMouse;

    float currentPitch = 0;

    unsigned int screenWidth, screenHeight;
};


#endif
