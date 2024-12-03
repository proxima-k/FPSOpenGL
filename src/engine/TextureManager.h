#pragma once

#include <glew/glew.h>
#include <stb/stb_image.h>
#include <iostream>
#include <unordered_map>
#include <filesystem>

class TextureManager
{
public:
	std::unordered_map<std::string, GLuint> textures;

	void init();
	void loadTexture(std::string textureName);

	GLuint getTexture(std::string fileName)
	{
		if (textures.find(fileName) != textures.end()) {
			return textures[fileName];
		}

		return 0;
	}
};

extern TextureManager* textureManager;