#pragma once

#include<glew/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <stb/stb_image.h>
#include <iostream>


class PlayerUI
{
public:
	void update(GLFWwindow* window, float deltaTime);
	void loadImage(const char* imagePath) 
    {
        int nrChannels;
        unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
        if (data) {

            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            if (nrChannels == 3)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            else if (nrChannels == 4)
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            else {
                std::cerr << "Unsupported image format: " << nrChannels << " channels." << std::endl;
            }

            glGenerateMipmap(GL_TEXTURE_2D);

            stbi_image_free(data);
        }
        else {
            std::cerr << "Failed to load texture: " << imagePath << std::endl;
        }
	}

private: 
	GLuint textureID;
	int width, height;
};