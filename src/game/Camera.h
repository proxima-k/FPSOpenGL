#ifndef CAMERA_H
#define CAMERA_H

#include <glew/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

#include "Transform.h"

class Camera {
public:
    static Camera* mainCamera;
    Transform transform;
    
    Camera(glm::vec3 worldUp, unsigned int screenWidth, unsigned int screenHeight);

    glm::vec3 getWorldUp() const { return worldUp; }

    void updateScreenSize(unsigned int width, unsigned int height) { screenWidth = width; screenHeight = height; }

    float getResolutionRatio() { return (float)screenWidth / screenHeight; }
    unsigned int getScreenWidth() { return screenWidth; }
    unsigned int getScreenHeight() { return screenHeight; }

private:
    // todo: change to worldUp?
    glm::vec3 worldUp;

    unsigned int screenWidth, screenHeight;
};

#endif
/*
   //void processMouseMovement(float xPos, float yPos);
   //glm::vec3 getCameraForward() const { return cameraForward; }
   glm::vec3 cameraForward;
   float lastX;
   float lastY;
   bool firstMouse;

   float currentPitch = 0;
*/